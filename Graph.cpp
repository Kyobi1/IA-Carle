#include "Graph.h"
#include <iterator>
#include <algorithm>

const std::string Node::typeNames[3] = { "Goal", "Forbidden", "Default" };

void Graph::init(const SInitData& initData) 
{
	colCount = initData.colCount;
	rowCount = initData.rowCount;


	initMapDefaultValues(initData);
	initObjectsConnections(initData);
	updateUtilityScores();
}

void Graph::update(const STurnData& turnData)
{

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


void Graph::updateUtilityScore(HexCell const& graphKey)
{
	auto& node = map[graphKey];
	node.utilityScore = std::count_if(std::begin(node.connections), std::end(node.connections),
		[](Connection const& connex) { return connex.object == Connection::Unknown; }
	);
}

void Graph::updateUtilityScores()
{
	for (auto& pair : map)
	{
		updateUtilityScore(pair.first);
	}
}

void Graph::debug(Logger& logger) const
{
	logger.Log("Infos graphe : ");
	logger.Log("Nombre de noeuds : " + std::to_string(map.size()) + '\n');
	std::for_each(begin(map), end(map), [&logger](std::pair<graphKey, Node> n) { logger.Log("Node : "); n.second.debug(logger); });
}