#ifndef PATHFINDERASTAR_H
#define PATHFINDERASTAR_H

#include "PathFinder.h"
#include <unordered_map>

class Graph;

class PathfinderAStar : public PathFinder
{
	std::unordered_map<HexCell, NodeRecord> Nodes;
	CellKey start;

	int heuristicEuclidian(const HexCell& node, const HexCell& goal) const;
	path getPath(NodeRecord* record, const HexCell& goal) const;
	void reset();


public:
	PathfinderAStar(const Graph& graph, const HexCell& start);
	bool operator==(const PathfinderAStar& other);

	void setStart(const HexCell&);

	struct Reset {
		void operator()(PathfinderAStar& pathfinder) { pathfinder.reset(); };
	};
	static Reset resetPathfinder;

	path compute(const HexCell& goal) override;
};

#endif // !PATHFINDERASTAR_H

