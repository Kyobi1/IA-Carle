#pragma once
#include "HexCell.h"
#include <string>
//#include "StateMachine.h"
class StateMachine;
class State;
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
public:
	NPC();
	NPC(int id_, cellType startPos, int visionRange_, stateTypes etat_ = NON_ASSIGNE);
	NPC(const NPC& npc_);
	~NPC();
	void initStateMachine(State* startState, int numNPC);
	void updateStateMachine();
	stateTypes getEtat() const;

	void debug(Logger& logger) const;
};