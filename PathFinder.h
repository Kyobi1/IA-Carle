#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "PriorityHeap.h"
#include "NodeReccord.h"

#include <vector>
#include <utility>

class Graph;

class PathFinder {
protected:
	using CellKey = HexCell;
	using PathfindingList = PriorityHeap<NodeRecord>;
	using path = std::vector<HexCell>;

	PathfindingList openSet;
	const Graph* graph;
	//PathfindingList closeSet;

	virtual path compute(const HexCell& start) = 0;
protected:
	PathFinder(const Graph&);

public:
	//PathFinder() = default;

	//std::vector<CellKey> getPath();

};


#endif // !PATHFINDER_H

