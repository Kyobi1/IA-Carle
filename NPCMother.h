#pragma once
#include "NPC.h"
#include <vector>

class MAP
{

};

class NPCMother
{
	using cellType = Cell;
	std::vector<NPC> enfants;
	MAP* map;
public:
	NPCMother();
	NPCMother(std::vector<cellType> NPCs, MAP* map_);
};