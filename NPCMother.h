#pragma once
#include "NPC.h"
#include "Graph.h"
#include <vector>

class State;
class NPCMother
{
	using cellType = HexCell;
	std::vector<NPC> enfants;
	Graph* map;
	std::vector<State*> etats;

	void createStateMachine();
public:
	NPCMother() = default;
	NPCMother(const std::vector<NPC>& NPCs, Graph* map_);
	NPCMother(const NPCMother&) = delete;
	~NPCMother();
	void init(const SInitData& _initData, Graph* map_);

	bool resteAssezDeTemps(int numNPC) const;
	bool NPCSTousArrives() const;
	bool NPCAUneCible(int numNPC) const;
	bool NPCCibleAtteinte(int numNPC) const;
	bool NPCEchangeCible(int numNPC) const;

	bool tabAssezDeTemps[10] = { false, true, false, true, true, false, false, false, false, false };
	bool tabNPCSTousArrives[10] = { true, true, false, false, true, false, true, false, false, false };
	bool tabNPCAUneCible[10] = { true, false, false, false, false, false, false, false, false, false };
	bool tabNPCCibleAtteinte[10] = { false, true, false, false, true, false, false, false, false, false };
	bool tabNPCEchangeCible[10] = { false, false, false, false, false, true, false, false, false, false };

	int num = 0;
	void nextTurn();

	void debug(Logger& logger) const;
};