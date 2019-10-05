#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "PriorityHeap.h"
#include "NodeReccord.h"

#include <vector>
#include <utility>

class PathFinder {
	using CellKey = int;
	using PathfindingList = PriorityHeap<NodeReccord>;

	PathfindingList openSet;
	PathfindingList closeSet;

	//void compute();
public:
	class NoPathFound {};
	PathFinder() = default;
	//std::vector<CellKey> getPath();

};


#endif // !PATHFINDER_H

