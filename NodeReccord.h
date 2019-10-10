#ifndef NODE_RECCORD_H
#define NODE_RECCORD_H

struct NodeReccord {
	using NodePtr = int*;
	using CostValue = uint16_t;

	NodePtr node;
	NodePtr from;
	
	CostValue costSoFar;
	CostValue estimatedCost;
};

#endif // !NODE_RECCORD_H

