#pragma once
#include "HexCell.h"
#include "Logger.h"
#include <string>

struct Task;
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
	cellType temporaryGoalTile = HexCell{-1,-1};
	cellType turnDestination;
	bool hasGoal;

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
	void setEtat(stateTypes stateType);
	void giveGoal(const cellType& temporaryGoalTile_);
	void removeGoal();
	bool getHasGoal() const;
	void setHasGoal(bool hasGoal_);
	int getVisionRange() const;

	void debug(Logger& logger) const;
};