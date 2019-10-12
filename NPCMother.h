#pragma once
#include "NPC.h"
#include "Graph.h"
#include <vector>

class NPCMother
{
	using cellType = HexCell;
	std::vector<NPC> enfants;
	Graph* map;
public:
	NPCMother() = default;
	NPCMother(const std::vector<NPC>& NPCs, Graph* map_);
};