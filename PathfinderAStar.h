#ifndef PATHFINDERASTAR_H
#define PATHFINDERASTAR_H

#include "PathFinder.h"
#include "Graph.h"

class PathfinderAStar : public PathFinder
{
	std::unordered_map<HexCell, NodeReccord> Nodes;
	CellKey start;

	int heuristicEuclidian(const HexCell& node);

public:
	PathfinderAStar(const Graph& graph, HexCell start);

	path compute(const HexCell& goal) override;
};

#endif // !PATHFINDERASTAR_H

