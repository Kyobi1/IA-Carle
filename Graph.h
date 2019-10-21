#pragma once
#include <unordered_map>
#include <vector>
#include "Connection.h"
#include "TurnData.h"
#include "InitData.h"
#include "Logger.h"
//#include "PathFinder.h"
#include "Globals.h"
#include "PoolService.h"
#include "PathfinderAStar.h"

#include <algorithm>
#include <sstream>
#include <memory>


struct Node {
	std::vector<Connection> connections;
	STileInfo nodeInfos;
	float utilityScore = 0;
	const static std::string typeNames[3];

	using PathfinderPool = PoolService<PathfinderAStar, decltype(PathfinderAStar::resetPathfinder)>;
	using PathfinderPtr = PathfinderPool::pooled_ptr;
	PathfinderPtr pathFinder{};

	void debug(Logger& logger) const
	{
		std::stringstream ss;
		ss << "q : " << nodeInfos.q << "\tr : " << nodeInfos.r << std::endl;
		ss << "type : " << typeNames[nodeInfos.type] << std::endl;
		ss << "Nombre de voisins : " << connections.size() << std::endl;
		ss << "Utility Score : " << utilityScore <<  std::endl;
		logger.Log(ss.str());
		std::for_each(begin(connections), end(connections), [&logger](Connection connection) { logger.Log("Connection : "); connection.debug(logger); });
	}
};

class Graph {
public:
	using graphKey = HexCell;
	using orderList = std::vector<SOrder>;
private:
	using PathfinderPool = Node::PathfinderPool;

	using PathfinderPoolPtr = std::unique_ptr<PathfinderPool>;
	PathfinderPoolPtr pathfinders;
	std::unordered_map<graphKey, Node> map;
	int colCount;
	int rowCount;

public:
	
	Graph() = default;

	void init(const SInitData& initData);
	void update(const STurnData& turnData);

	const Node* getNode(const graphKey& key) const;
	const std::unordered_map<graphKey, Node>& getNodes() const;

	PathFinder::path getPath(const HexCell& from, const HexCell& to);

	std::vector<STileInfo> removeForbiddenTiles(STileInfo* tileInfoArray, int tileInfoArraySize) const;

	void initMapDefaultValues(SInitData const& initData);
	void initObjectsConnections(SInitData const& initData);

	void updateConnections(STurnData const& turnData);

	void updateUtilityScore(HexCell const& graphKey);
	void updateUtilityScores();

	SOrder getOrder(EHexCellDirection dir, int uid);

	void debug(Logger& logger) const;
};
