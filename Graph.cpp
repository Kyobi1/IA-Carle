#include "Graph.h"
#include <iterator>
#include <algorithm>

#include <cassert>

#include "PathfinderAStar.h"

const std::string Node::typeNames[3] = { "Goal", "Forbidden", "Default" };

void Graph::init(const SInitData& initData) 
{
	colCount = initData.colCount;
	rowCount = initData.rowCount;

	initMapDefaultValues(initData);
	initObjectsConnections(initData);
	updateUtilityScores();

	pathfinders = std::make_unique<PathfinderPool>(10, PathfinderAStar::resetPathfinder, *this, HexCell(0, 0));
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

void Graph::initMapDefaultValues(SInitData const& initData)
{
	STileInfo* tiles = initData.tileInfoArray;
	std::for_each(tiles, tiles + initData.tileInfoArraySize, [this, &initData](STileInfo tile) {
		graphKey cell(tile.q, tile.r);
		std::vector<graphKey> neighbors = cell.getNeighbors();
		for (int j = 0; j < neighbors.size(); ++j)
		{
			map[cell].connections.emplace_back(cell, neighbors[j], Connection::Forbidden);
		}
		map[cell].nodeInfos = tile;
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
	for (int i = 0; i < initData.objectInfoArraySize; ++i)
	{
		HexCell cell(initData.objectInfoArray[i].q, initData.objectInfoArray[i].r);
		HexCell neighbor(cell.neighborFromDirection(initData.objectInfoArray[i].cellPosition));
		map[cell].connections[initData.objectInfoArray[i].cellPosition]
			.setObjectType(static_cast<Connection::ObjectType>(initData.objectInfoArray[i].types[0]));
		map[neighbor].connections[HexCell::oppositeDirection(initData.objectInfoArray[i].cellPosition)]
			.setObjectType(static_cast<Connection::ObjectType>(initData.objectInfoArray[i].types[0]));
	}
}

void Graph::updateConnections(STurnData const& turnData)
{
	std::for_each(turnData.tileInfoArray, turnData.tileInfoArray + turnData.tileInfoArraySize, [this](STileInfo& data)
	{
			HexCell cell{ data.q, data.r };
			for (auto& connection : map[cell].connections)
			{
				if (connection.object == Connection::Unknown && map[connection.destinationNode].nodeInfos.type == Forbidden)
				{
					connection.setObjectType(Connection::Forbidden);
				}
			}
	});

	for (int i = 0; i < turnData.objectInfoArraySize; ++i)
	{
		HexCell cell(turnData.objectInfoArray[i].q, turnData.objectInfoArray[i].r);
		HexCell neighbor(cell.neighborFromDirection(turnData.objectInfoArray[i].cellPosition));

		map[cell].connections[turnData.objectInfoArray[i].cellPosition]
			.setObjectType(static_cast<Connection::ObjectType>(turnData.objectInfoArray[i].types[0]));
		map[neighbor].connections[HexCell::oppositeDirection(turnData.objectInfoArray[i].cellPosition)]
			.setObjectType(static_cast<Connection::ObjectType>(turnData.objectInfoArray[i].types[0]));

	}
	updateUtilityScores();
}


void Graph::updateUtilityScore(HexCell const& graphKey)
{
	auto& node = map[graphKey];
	node.utilityScore = static_cast<float>(std::count_if(std::begin(node.connections), std::end(node.connections),
		[](Connection const& connex) { return connex.object == Connection::Unknown; }
	));
	if (node.utilityScore < 1) node.pathFinder.reset();
}

void Graph::updateUtilityScores()
{
	for (auto& pair : map)
	{
		updateUtilityScore(pair.first);
	}
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

