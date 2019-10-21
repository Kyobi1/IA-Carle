#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "PriorityHeap.h"
#include "NodeReccord.h"

#include <vector>
#include <utility>

class Graph;

class PathFinder {
public:
	using path = std::vector<HexCell>;
protected:
	using CellKey = HexCell;
	using PathfindingList = PriorityHeap<NodeRecord>;

	PathfindingList openSet;
	const Graph* graph;
	//PathfindingList closeSet;
protected:
	PathFinder(const Graph&);

public:
	virtual path compute(const HexCell& start) = 0;


};


#endif // !PATHFINDER_H

