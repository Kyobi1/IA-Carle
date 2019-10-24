#include "Graph.h"
#include <iterator>
#include <algorithm>
#include <unordered_set>

#include <cassert>

#include "Debug.h"
#include "PathfinderAStar.h"

const std::string Node::typeNames[3] = { "Goal", "Forbidden", "Default" };



std::vector<HexCell> Graph::getSortedReachableCells(const graphKey& origin, int radius) const
{
	std::vector<HexCell> reachable{};
	std::vector<std::vector<HexCell>> fringe{ { origin } };

	for (int i = 1; i < radius + 1; ++i)
	{
		fringe.push_back({});
		for(auto& cell : fringe[i - 1])
		{ 
			for (int dir = EHexCellDirection::W; dir != CENTER; ++dir) {
				auto n = cell.getNeighborFromDirection(static_cast<EHexCellDirection>(dir));
				if (n == origin) continue;
				if (map.at(cell).connections[dir].object == Connection::Nothing)
				{
					auto& debug = map.at(n);
					reachable.push_back(n);
					fringe[i].push_back(n);
				}
			}
		}
	}

	std::sort(begin(reachable), end(reachable), [this](const auto& a, const auto& b) { 
		if (map.at(a).utilityScore == map.at(b).utilityScore) return a < b;
		return map.at(a).utilityScore > map.at(b).utilityScore;
		});
	reachable.erase(std::unique(begin(reachable), end(reachable)), end(reachable));
	//std::sort(begin(reachable), end(reachable), [this](const auto& a, const auto& b) { return map.at(a).utilityScore > map.at(b).utilityScore; });
	return reachable;
}

void Graph::init(const SInitData& initData)
{
	colCount = initData.colCount;
	rowCount = initData.rowCount;

	initDiscoveredMapDefaultValues(initData);
	initObjectsConnections(initData);
	updateUtilityScores();

	pathfinders = std::make_unique<PathfinderPool>(20, PathfinderAStar::resetPathfinder, *this, HexCell(0, 0));
}

void Graph::update(const STurnData& turnData)
{
	updateConnections(turnData);
}

std::vector<STileInfo> Graph::removeForbiddenTiles(STileInfo* tileInfoArray, int tileInfoArraySize) const
{
	std::vector<STileInfo> tiles;
	std::for_each(tileInfoArray + 0, tileInfoArray + tileInfoArraySize, [&tiles](STileInfo tile) { if (tile.type != EHexCellType::Forbidden) tiles.push_back(tile); });
	return tiles;
}

const Node* Graph::getNode(const graphKey& key) const
{
	return &map.at(key);
}

const std::unordered_map<Graph::graphKey, Node>& Graph::getNodes() const
{
	return map;
}

PathFinder::path Graph::getPath(const HexCell& from, const HexCell& to)
{
	assert((map.find(to) != map.end()));
	if (!map[to].pathFinder.get()) {
		try {
			map[to].pathFinder = pathfinders->request();
		} 
		catch(PathfinderPool::NoneAvailable&) {
			pathfinders->increaseSize(1, *this, to);
			map[to].pathFinder = pathfinders->request();
		}
		map[to].pathFinder->setStart(to);
	}
	return map[to].pathFinder->compute(from);
	
}

void Graph::initDiscoveredMapDefaultValues(SInitData const& initData)
{
	STileInfo* tiles = initData.tileInfoArray;
	std::for_each(tiles, tiles + initData.tileInfoArraySize, [this, &initData](STileInfo tile) {
		graphKey cell(tile.q, tile.r);
		if (tile.type == Goal) goals.push_back(cell);
		std::vector<graphKey> neighbors = cell.getNeighbors();
		for (int j = 0; j < neighbors.size(); ++j)
		{
			map[cell].connections.emplace_back(cell, neighbors[j], Connection::Forbidden);//setObjectType(Connection::Forbidden);
		}
		map[cell].nodeInfos = tile;
		map[cell].state = Node::Discovered;

	});
}

void Graph::initObjectsConnections(SInitData const& initData)
{
	for (std::unordered_map<graphKey, Node>::iterator it = map.begin(); it != map.end(); ++it)
	{
		for (int i = 0; i < it->second.connections.size(); ++i)
		{
			if (map.find(it->second.connections[i].destinationNode) != map.end())
			{
				if (map[it->second.connections[i].destinationNode].nodeInfos.type != Forbidden)
					it->second.connections[i].setObjectType(Connection::Nothing);
			}
			else if (!it->second.connections[i].destinationNode.isOutOfBound(colCount, rowCount))
				it->second.connections[i].setObjectType(Connection::Unknown);
		}
	}

	initUndiscoveredDefaultValues(initData);

	for (int i = 0; i < initData.objectInfoArraySize; ++i)
	{
		HexCell cell(initData.objectInfoArray[i].q, initData.objectInfoArray[i].r);
		HexCell neighbor(cell.getNeighborFromDirection(initData.objectInfoArray[i].cellPosition));
		Debug::Log(initData.objectInfoArray[i].types[0]);
		if(!cell.isOutOfBound(colCount, rowCount))
			map[cell].connections[initData.objectInfoArray[i].cellPosition]
				.setObjectType(static_cast<Connection::ObjectType>(initData.objectInfoArray[i].types[0]));
		if (!neighbor.isOutOfBound(colCount, rowCount))
			map[neighbor].connections[HexCell::oppositeDirection(initData.objectInfoArray[i].cellPosition)]
				.setObjectType(static_cast<Connection::ObjectType>(initData.objectInfoArray[i].types[0]));
	}
}

void Graph::initUndiscoveredDefaultValues(SInitData const& initData)
{
	for (int q = 0; q < rowCount; ++q)
	{
		int offset = q / 2;
		for (int r = -offset; r < colCount - offset; ++r)
		{
			graphKey cell(q, r);
			if (map.find(cell) != end(map)) continue;
			
			std::vector<graphKey> neighbors = cell.getNeighbors();
			for (int j = 0; j < neighbors.size(); ++j)
			{
				map[cell].connections.emplace_back(cell, neighbors[j], Connection::Unknown);
				if (map.find(neighbors[j]) != end(map))
				{
					map[neighbors[j]].connections[HexCell::oppositeDirection(static_cast<EHexCellDirection>(j))]
						.setObjectType(Connection::Unknown);//(neighbors[j], cell, Connection::Unknown);
				}
			}

			map[cell].nodeInfos.r = r;
			map[cell].nodeInfos.q = q;
			map[cell].nodeInfos.type = Default;
		}
	}
}

void Graph::updateConnections(STurnData const& turnData)
{
	std::for_each(turnData.tileInfoArray, turnData.tileInfoArray + turnData.tileInfoArraySize, [this](STileInfo& data)
	{
			HexCell cell{ data.q, data.r };
			if (data.type == Goal) goals.push_back(cell);
			for (auto& connection : map[cell].connections)
			{
				if (connection.object == Connection::Unknown && map[connection.destinationNode].nodeInfos.type == Forbidden)
				{
					connection.setObjectType(Connection::Forbidden);
					updateUtilityScore(connection.originNode);
					updateUtilityScore(connection.destinationNode);
				}
			}
			map[cell].state = Node::Discovered;
	});

	for (int i = 0; i < turnData.objectInfoArraySize; ++i)
	{

		HexCell cell(turnData.objectInfoArray[i].q, turnData.objectInfoArray[i].r);
		HexCell neighbor(cell.getNeighborFromDirection(turnData.objectInfoArray[i].cellPosition));
		if (!cell.isOutOfBound(colCount, rowCount))
		{
			map[cell].connections[turnData.objectInfoArray[i].cellPosition]
				.setObjectType(static_cast<Connection::ObjectType>(turnData.objectInfoArray[i].types[0]));
			updateUtilityScore(cell);
		}
			
		if (!neighbor.isOutOfBound(colCount, rowCount))
		{
			map[neighbor].connections[HexCell::oppositeDirection(turnData.objectInfoArray[i].cellPosition)]
				.setObjectType(static_cast<Connection::ObjectType>(turnData.objectInfoArray[i].types[0]));
			updateUtilityScore(neighbor);
		}

	}
}


void Graph::updateUtilityScore(HexCell const& graphKey)
{
	auto& node = map[graphKey];
	int nbUnknown{0};
	int nbWalls{0};
	int nbWindows{0};
	std::for_each(begin(node.connections), end(node.connections),
		[&](const Connection& c)
		{
			switch (c.object) {
			case Connection::Wall:
				++nbWalls; break;
			case Connection::Unknown:
				++nbUnknown; break;
			case Connection::Window:
				if(map.at(c.destinationNode).state == Node::Undiscovered)
					++nbWindows;
				break;
			default: break;
			}
		});

	node.utilityScore = nbUnknown + nbWindows * 0.8f + nbWalls * 0.2f;
	//if (node.utilityScore < 1) node.pathFinder.reset();
}

void Graph::updateUtilityScores()
{
	for (auto& pair : map)
	{
		updateUtilityScore(pair.first);
	}
}

auto Graph::getHighestUtilityCell(const graphKey& origin, int radius) const -> graphKey
{
	auto reachableCells = getSortedReachableCells(origin, radius);
	return reachableCells.front();
}

const std::vector<HexCell>& Graph::getDiscoveredGoals()
{
	std::sort(begin(goals), end(goals));
	goals.erase(std::unique(begin(goals), end(goals)), end(goals));
	return goals;
}


SOrder Graph::getOrder(EHexCellDirection dir, int uid)
{
	SOrder order;
	order.orderType = EOrderType::Move;
	order.direction = dir;
	order.npcUID = uid;
	return order;
}

void Graph::debug(Logger& logger) const
{
	logger.Log("Infos graphe : ");
	logger.Log("Nombre de noeuds : " + std::to_string(map.size()) + '\n');
	std::for_each(begin(map), end(map), [&logger](const std::pair<const graphKey, Node>& n) { logger.Log("Node : "); n.second.debug(logger); });
}

