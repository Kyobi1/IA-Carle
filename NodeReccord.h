#ifndef NODE_RECCORD_H
#define NODE_RECCORD_H

#include "HexCell.h"
#include "Connection.h"
struct NodeRecord {
	using Node = HexCell;
	using costValue = Connection::costValue;

	Node node;
	NodeRecord* from;
	
	costValue costSoFar;
	costValue estimatedCost;

	enum { CLOSED, OPEN, UNVISITED } category;

	NodeRecord() = default; //TODO
	NodeRecord(const HexCell& node) : node{ node }, from{ nullptr }, costSoFar{ 0 }, estimatedCost{ Connection::MAX_COST_VALUE }, category{ UNVISITED }{}
	friend bool operator<(const NodeRecord& a,const NodeRecord& b) { return b.estimatedCost < a.estimatedCost; }
	bool operator==(const NodeRecord& nodeCompare) { return node == nodeCompare.node; }
};

#endif // !NODE_RECCORD_H

