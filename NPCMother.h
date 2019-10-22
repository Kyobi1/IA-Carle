#pragma once
#include "NPC.h"
#include "Graph.h"
#include <vector>

class State;
class NPCMother
{
	using cellType = HexCell;
	std::vector<NPC> enfants;
	std::vector<SOrder> ordersChilds;
	Graph* map;
	std::vector<State*> etats;
	std::vector<HexCell> goalsDiscovered;

	void createStateMachine();
	void initGoals();
	int getIndexNPCFromId(int idNPC) const;
public:
	NPCMother() = default;
	NPCMother(const std::vector<NPC>& NPCs, Graph* map_);
	NPCMother(const NPCMother&) = delete;
	~NPCMother();
	void init(const SInitData& _initData, Graph* map_);
	void giveOrders(std::list<SOrder>& _orders);

	HexCell getGoalNPC(int idNPC) const;

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