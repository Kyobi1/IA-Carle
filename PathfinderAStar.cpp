#include "PathfinderAStar.h"
#include <algorithm>
#include <iterator>

PathfinderAStar::PathfinderAStar(const Graph& graph, HexCell start) : start{start}
{
	const std::unordered_map<Graph::graphKey, Node>& nodesGraph = graph.getNodes();
	std::for_each(begin(nodesGraph), end(nodesGraph), [this](std::pair<Graph::graphKey, Node> node) {
		NodeReccord nodeRecord;
		nodeRecord.node = &node.first;
		nodeRecord.category = NodeReccord::UNVISITED;
		nodeRecord.costSoFar = 0;
		nodeRecord.estimatedCost = 0;
		nodeRecord.from = nullptr;
		Nodes[*nodeRecord.node] = nodeRecord;
	});
}

int PathfinderAStar::heuristicEuclidian(const HexCell& node)
{
	return start.distanceTo(node);
}

PathfinderAStar::path PathfinderAStar::compute(const HexCell& goal)
{
	if (Nodes[start].category == NodeReccord::UNVISITED)
	{
		Nodes[start].category == NodeReccord::OPEN;
		openSet += Nodes[start];
		Nodes[start].costSoFar = 0;
		Nodes[start].estimatedCost = heuristicEuclidian(goal);
	}

	HexCell cur;
}