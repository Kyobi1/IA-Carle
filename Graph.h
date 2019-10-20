#pragma once
#include <unordered_map>
#include <vector>
#include "Connection.h"
#include "TurnData.h"
#include "InitData.h"
#include "Logger.h"
#include <algorithm>
#include <sstream>

#include "Globals.h"

struct Node {
	std::vector<Connection> connections;
	STileInfo nodeInfos;
	float utilityScore;
	const static std::string typeNames[3];

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
private:
	std::unordered_map<graphKey, Node> map;
	int colCount;
	int rowCount;


public:
	
	Graph() = default;

	void init(const SInitData& initData);
	void update(const STurnData& turnData);

	const Node* getNode(const graphKey& key) const;
	const std::unordered_map<graphKey, Node>& getNodes() const;

	std::vector<STileInfo> removeForbiddenTiles(STileInfo* tileInfoArray, int tileInfoArraySize) const;

	void initMapDefaultValues(SInitData const& initData);
	void initObjectsConnections(SInitData const& initData);

	void updateUtilityScore(HexCell const& graphKey);
	void updateUtilityScores();

	void debug(Logger& logger) const;
};
