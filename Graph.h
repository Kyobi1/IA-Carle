#pragma once
#include <unordered_map>
#include <vector>
#include "Connection.h"
#include "TurnData.h"
#include "InitData.h"
#include <algorithm>

struct Node {
	std::vector<Connection> connections;
	STileInfo* nodeInfos;
};

class Graph {
	using graphKey = HexCell;

	std::unordered_map<graphKey, Node> map;

public:
	
	Graph() = default;

	void init(const SInitData& initData);
	void update(const STurnData& turnData);

	Node* getNode(const graphKey& key) const;
};
