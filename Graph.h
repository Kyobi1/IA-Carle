#pragma once
#include <unordered_map>
#include <vector>
#include "Connection.h"
#include "TurnData.h"
#include "InitData.h"
#include "Logger.h"
#include <algorithm>
#include <sstream>


struct Node {
	std::vector<Connection> connections;
	STileInfo nodeInfos;
	const static std::string typeNames[3];

	void debug(Logger& logger) const
	{
		std::stringstream ss;
		ss << "q : " << nodeInfos.q << "\tr : " << nodeInfos.r << std::endl;
		ss << "type : " << typeNames[nodeInfos.type] << std::endl;
		ss << "Nombre de voisins : " << connections.size() << std::endl;
		logger.Log(ss.str());
		std::for_each(begin(connections), end(connections), [&logger](Connection connection) { logger.Log("Connection : "); connection.debug(logger); });
	}
};

class Graph {
public:
	using graphKey = HexCell;
private:
	std::unordered_map<graphKey, Node> map;

	std::vector<STileInfo> removeForbiddenTiles(STileInfo* tileInfoArray, int tileInfoArraySize) const;

public:
	
	Graph() = default;

	void init(const SInitData& initData);
	void update(const STurnData& turnData);

	Node* getNode(const graphKey& key) const;
	const std::unordered_map<graphKey, Node>& getNodes() const;

	void debug(Logger& logger) const;
};
