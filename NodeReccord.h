#ifndef NODE_RECCORD_H
#define NODE_RECCORD_H

#include "HexCell.h"

struct NodeReccord {
	using NodePtr = HexCell*;
	using CostValue = unsigned short;

	NodePtr node;
	NodePtr from;
	
	CostValue costSoFar;
	CostValue estimatedCost;

	enum { CLOSED, OPEN, UNVISITED } category;

	bool operator<(const NodeReccord& nodeCompare) { return estimatedCost > nodeCompare.estimatedCost; }
};

#endif // !NODE_RECCORD_H

