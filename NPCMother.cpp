#include "NPCMother.h"
#include <algorithm>
#include <iterator>

NPCMother::NPCMother() : map(nullptr)
{

}

NPCMother::NPCMother(std::vector<cellType> NPCs, MAP* map_) : map(map_)
{
	std::for_each(begin(NPCs), end(NPCs), [this](cellType cell) {enfants.emplace_back(cell); });
}