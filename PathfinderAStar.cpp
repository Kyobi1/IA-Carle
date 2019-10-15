#include "PathfinderAStar.h"
#include <algorithm>
#include <iterator>
#include "Debug.h"

PathfinderAStar::PathfinderAStar(const Graph& graph, const HexCell& start) : PathFinder{ graph }, start{ start }
{
	const std::unordered_map<Graph::graphKey, Node>& nodesGraph = graph.getNodes();
	
	std::for_each(begin(nodesGraph), end(nodesGraph), [this](std::pair<Graph::graphKey, Node> node) {
		NodeRecord nodeRecord{ node.first };
		Nodes.emplace(node.first, nodeRecord);
	});
	
}

int PathfinderAStar::heuristicEuclidian(const HexCell& node, const HexCell& goal) const
{
	return node.distanceTo(goal);
}

auto PathfinderAStar::getPath(NodeRecord* record, const HexCell& goal) const ->path
{
	path p;
	auto cur = record;
	do {
		p.push_back(cur->node);
		cur = cur->from;
	} while (cur->node != start);
	
	p.push_back(start);
	return p;
}

PathfinderAStar::path PathfinderAStar::compute(const HexCell& goal)
{

	if (Nodes[start].category == NodeRecord::UNVISITED)
	{
		Nodes[start].category = NodeRecord::OPEN;
		openSet += Nodes[start];
		Nodes[start].costSoFar = 0;
		Nodes[start].estimatedCost = heuristicEuclidian(start, goal);
	}
	
	
	NodeRecord* current;
	while (Nodes[goal].category != NodeRecord::CLOSED)
	{
		if (openSet.empty()) break;

		current = &Nodes[openSet.smallestElement().node]; //TODO modifier openSet pour qu'il prenne des pointeurs
		//current = &Nodes[current->node];

		if (current->node == goal) { return getPath(current, start); }
		
		current->category = NodeRecord::CLOSED;
		openSet -= *current;
		auto v = graph->getNode(current->node)->connections;

		NodeRecord::costValue d;
		for (auto& connex : graph->getNode(current->node)->connections)
		{
			if (connex.object != Connection::PressurePlate && connex.object != Connection::Nothing) continue;

			if (Nodes[connex.destinationNode].category == NodeRecord::CLOSED) continue; //Attention poids plus grands que un sinon il faut remettre dans le openSet (consistent heuristique)
			
			d = current->costSoFar + connex.getCost();
			
			if (Nodes[connex.destinationNode].category == NodeRecord::OPEN && d >= Nodes[connex.destinationNode].costSoFar)
			{
				continue;
			}
			Nodes[connex.destinationNode].costSoFar = d;
			Nodes[connex.destinationNode].estimatedCost = d + heuristicEuclidian(connex.destinationNode, goal);
			Nodes[connex.destinationNode].from = current;//&Nodes[connex.originNode];;
			
			if (Nodes[connex.destinationNode].category != NodeRecord::OPEN) {
				openSet += Nodes[connex.destinationNode];
			}
		}

	}
	Debug::Log("no path found");
	return path{};
}