#include "NPCMother.h"
#include <algorithm>
#include <iterator>

NPCMother::NPCMother(const std::vector<NPC>& NPCs, Graph* map_) : map(map_), enfants(NPCs)
{
	
}