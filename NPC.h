#pragma once
#include "HexCell.h"
#include "Logger.h"
#include <string>

class Task;
class StateMachine;
class State;
class NPCMother;
class NPC
{
public:
	enum stateTypes { EXPLORATION, DEPLACEMENT_CIBLE, EN_ATTENTE, ARRIVE, NON_ASSIGNE };
	const static std::string stateNames[5];
private:
	using cellType = HexCell;
	cellType pos;
	stateTypes etat;
	int id;
	int visionRange;
	StateMachine* stateMachine;
	Task* taskMove;
	cellType temporaryGoalTile;
	cellType turnDestination;
	bool hasFinalGoal;

	Logger logger;
	
public:
	NPC();
	NPC(int id_, cellType startPos, int visionRange_, stateTypes etat_ = NON_ASSIGNE);
	NPC(const NPC& npc_);
	~NPC();
	void initStateMachine(State* startState, int numNPC);
	void updateStateMachine();
	stateTypes getEtat() const;
	int getId() const;
	cellType getPos() const;
	void avance(EHexCellDirection direction);

	cellType getTemporaryGoalTile() const;
	cellType getTurnDestination() const;
	void setTemporaryGoalTile(const cellType& temporaryGoalTile_);
	void setTurnDestination(const cellType& turnDestination_);
	void giveFinalGoal(const cellType& temporaryGoalTile_);
	void removeFinalGoal();
	bool getHasFinalGoal() const;

	void debug(Logger& logger) const;
};