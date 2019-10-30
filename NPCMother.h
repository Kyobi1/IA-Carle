#pragma once
#include "NPC.h"
#include "Graph.h"
#include "Logger.h"

#include <vector>

class State;
struct Task;
class NPCMother
{
	using cellType = HexCell;
	std::vector<NPC> enfants;
	std::vector<SOrder> ordersChilds;
	Graph* map;
	std::vector<State*> etats;
	bool solutionFound;
	std::vector<std::pair<int, PathFinder::path>> pathsNPC;
	std::vector<bool> canMove;

	void createStateMachine();
	int getIndexNPCFromId(int idNPC) const;
	int getIndexInSolutionNPCFromId(int idNPC) const;

	Logger logger;
	NPCMother() = default;

	Task* taskMove;
	Task* taskNoMove;
public:
	

	NPCMother(const NPCMother&) = delete;
	NPCMother& operator=(const NPCMother&) = delete;

	NPCMother(const std::vector<NPC>& NPCs, Graph* map_);
	~NPCMother();
	void init(const SInitData& _initData, Graph* map_);
	void giveOrders(std::list<SOrder>& _orders);
	NPC& getNPCByID(int idNPC);

	static NPCMother& getInstance()
	{
		static NPCMother mother;
		return mother;
	}

	HexCell getGoalNPC(int idNPC) const;
	HexCell getNextTile(int idNPC) const;
	void setGoalNPC(int idNPC);
	void setNextTile(int idNPC);
	void takeDecisons();
	void NPCAvance(int idNPC, EHexCellDirection direction);
	void NPCResteSurPlace(int idNPC);
	void solveConflicts();

	bool resteAssezDeTemps(int numNPC) const;
	bool NPCSTousArrives()const;
	bool NPCAUneCible(int numNPC)const;
	bool NPCCibleAtteinte(int numNPC) const;
	bool NPCEchangeCible(int numNPC) const;
	bool NPCArrive(int numNPC) const;

	void nextTurn();

	void debug(Logger& logger) const;
};