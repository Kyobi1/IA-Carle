#ifndef PATHFINDERASTAR_H
#define PATHFINDERASTAR_H

#include "PathFinder.h"
#include "Graph.h"

class PathfinderAStar : public PathFinder
{
	std::unordered_map<HexCell, NodeRecord> Nodes;
	CellKey start;

	int heuristicEuclidian(const HexCell& node, const HexCell& goal) const;
	path getPath(NodeRecord* record, const HexCell& goal) const;

public:
	PathfinderAStar(const Graph& graph, const HexCell& start);

	path compute(const HexCell& goal) override;
};

#endif // !PATHFINDERASTAR_H

