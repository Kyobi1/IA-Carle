#include "PathfinderAStar.h"
#include <algorithm>
#include <iterator>
#include "Debug.h"
#include "Graph.h"

PathfinderAStar::Reset PathfinderAStar::resetPathfinder;

PathfinderAStar::PathfinderAStar(const Graph& graph, const HexCell& start) : PathFinder{ graph }, start{ start }
{
	const std::unordered_map<Graph::graphKey, Node>& nodesGraph = graph.getNodes();
	
	std::for_each(begin(nodesGraph), end(nodesGraph), [this](const std::pair<const Graph::graphKey, Node>& node) {
		NodeRecord nodeRecord{ node.first };
		Nodes.emplace(node.first, nodeRecord);
	});
	
}

bool PathfinderAStar::operator==(const PathfinderAStar& other)
{
	return start == other.start;
}

void PathfinderAStar::setStart(const HexCell& start)
{
	this->start = start;
}

void PathfinderAStar::updateNodes(const Graph& graph)
{/*
	const std::unordered_map<Graph::graphKey, Node>& nodesGraph = graph.getNodes();
	Nodes.clear();
	openSet.clear();

	std::for_each(begin(nodesGraph), end(nodesGraph), [this](const std::pair<const Graph::graphKey, Node>& node) {
		NodeRecord nodeRecord{ node.first };
		Nodes.emplace(node.first, nodeRecord);
	});*/

	reset();

}

int PathfinderAStar::heuristicEuclidian(const HexCell& node, const HexCell& goal) const
{
	return node.distanceTo(goal);
}

auto PathfinderAStar::getPath(NodeRecord* record, const HexCell& goal) const ->path
{
	if (record->node == goal) return { };
	path p;
	auto cur = record;
	do {
		p.push_back(cur->node);
		cur = cur->from;
	} while (cur->node != start);
	
	p.push_back(start);
	std::reverse(begin(p), end(p));
	return p;
}

void PathfinderAStar::reset()
{
	for (auto& node : Nodes) { 
		node.second.category = NodeRecord::UNVISITED;
		node.second.costSoFar = 0;
		node.second.estimatedCost = Connection::MAX_COST_VALUE;
		node.second.from = nullptr;
	}

	openSet.clear();
}


PathfinderAStar::path PathfinderAStar::compute(const HexCell& goal)
{

	if (Nodes[start].category != NodeRecord::OPEN)
	{
		Nodes[start].category = NodeRecord::OPEN;
		openSet += Nodes[start];
		Nodes[start].costSoFar = 0;
		Nodes[start].estimatedCost = heuristicEuclidian(start, goal);
	}
	

	NodeRecord* current;
	while (Nodes[goal].category != NodeRecord::CLOSED)
	{
		if (openSet.empty()) {
			//Debug::Log("no path found");
			return path{};
		}
		current = &openSet.smallestElement();

		if (current->node == goal) { return getPath(current, start); }
		
		current->category = NodeRecord::CLOSED;
		openSet -= *current;

		NodeRecord::costValue d;
		for (auto const& connex : graph->getNode(current->node)->connections)
		{
			
			if (connex.object != Connection::Nothing ) continue;

			if (Nodes[connex.destinationNode].category == NodeRecord::CLOSED) { 

				continue; 
			} //Attention poids plus grands que un sinon il faut remettre dans le openSet (consistent heuristique)
			
			d = current->costSoFar + connex.getCost();
			if (Nodes[connex.destinationNode].category == NodeRecord::OPEN) {
				if (d >= Nodes[connex.destinationNode].costSoFar)
				{
					continue;
				}
			}
			else {
				Nodes[connex.destinationNode].category = NodeRecord::OPEN;

				Nodes[connex.destinationNode].costSoFar = d;
				Nodes[connex.destinationNode].estimatedCost = d + heuristicEuclidian(connex.destinationNode, goal);
				Nodes[connex.destinationNode].from = current;//&Nodes[connex.originNode];;
				openSet += Nodes[connex.destinationNode];

			}		
			
		}
		
		

	}

	return getPath(&Nodes[goal], start);
}