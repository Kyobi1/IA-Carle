#pragma once
#include "Map.h"
#include <unordered_map>
#include <vector>
#include "Connection.h"

struct Node {
	std::vector<Connection> connections;
	STileInfo nodeInfos;
};

class Graph {
	using key = HexCell;

	std::unordered_map<HexCell, Node> map;

};
