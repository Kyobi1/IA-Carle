#include "Graph.h"
#include <iterator>
#include <algorithm>

const std::string Node::typeNames[3] = { "Goal", "Forbidden", "Default" };

void Graph::init(const SInitData& initData)
{
	std::vector<STileInfo> tiles = removeForbiddenTiles(initData.tileInfoArray, initData.tileInfoArraySize);
	std::for_each(begin(tiles), end(tiles), [this, &initData](STileInfo tile) {
		graphKey cell(tile.q, tile.r);
		std::vector<graphKey> neighbors = cell.getNeighbors();
		for (int j = 0; j < neighbors.size(); ++j)
		{
			map[cell].connections.emplace_back(cell, neighbors[j], Connection::Forbidden);
		}
		map[cell].nodeInfos = tile;
	});
	for (std::unordered_map<graphKey, Node>::iterator it = map.begin(); it != map.end(); ++it)
	{
		for (int i = 0; i < it->second.connections.size(); ++i)
		{
			if (map.contains(it->second.connections[i].destinationNode)) it->second.connections[i].object = Connection::Nothing;
		}
	}
	for (int i = 0; i < initData.objectInfoArraySize; ++i)
	{
		HexCell cell(initData.objectInfoArray[i].q, initData.objectInfoArray[i].r);
		HexCell neighbor(cell.neighborFromDirection(initData.objectInfoArray[i].cellPosition));
		map[cell].connections[initData.objectInfoArray[i].cellPosition].object = static_cast<Connection::ObjectType>(initData.objectInfoArray[i].types[0]);
		map[neighbor].connections[HexCell::oppositeDirection(initData.objectInfoArray[i].cellPosition)].object = static_cast<Connection::ObjectType>(initData.objectInfoArray[i].types[0]);
	}
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

void Graph::debug(Logger& logger) const
{
	logger.Log("Infos graphe : ");
	logger.Log("Nombre de noeuds : " + std::to_string(map.size()) + '\n');
	std::for_each(begin(map), end(map), [&logger](std::pair<graphKey, Node> n) { logger.Log("Node : "); n.second.debug(logger); });
}