#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "PriorityHeap.h"
#include "NodeReccord.h"

#include <vector>
#include <utility>

class PathFinder {
protected:
	using CellKey = HexCell;
	using PathfindingList = PriorityHeap<NodeReccord>;
	using path = std::vector<HexCell>;

	PathfindingList openSet;
	//PathfindingList closeSet;

	virtual path compute(const HexCell& start) = 0;
public:
	class NoPathFound {};
	PathFinder() = default;

	//std::vector<CellKey> getPath();

};


#endif // !PATHFINDER_H

