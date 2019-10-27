#include "NPC.h"
#include "NPCMother.h"
#include "StateMachine.h"
#include "Task.h"


const std::string NPC::stateNames[5] = { "EXPLORATION", "DEPLACEMENT_CIBLE", "EN_ATTENTE", "ARRIVE", "NON_ASSIGNE" };

NPC::NPC() : id(-1), pos(), etat(NON_ASSIGNE), hasGoal(false)
{
	stateMachine = new StateMachine();
}

NPC::NPC(int id_, cellType startPos, int visionRange_, stateTypes etat_) : id(id_), pos(startPos), visionRange(visionRange_), etat(etat_), hasGoal(false)
{
	stateMachine = new StateMachine();
}

NPC::NPC(const NPC& npc_) : pos{ npc_.pos }, etat{ npc_.etat }, id{ npc_.id }, visionRange{ npc_.visionRange }, hasGoal{ false }
{
	stateMachine = new StateMachine(*(npc_.stateMachine));
}

NPC::~NPC()
{
	delete stateMachine;
}

void NPC::initStateMachine(State* startState, int numNPC)
{
	logger.Init("../Debug", "debugNPC.txt");
	stateMachine->setState(startState, numNPC);
}

void NPC::updateStateMachine()
{
	std::vector<Task*> actions = stateMachine->update();
	std::for_each(begin(actions), end(actions), [this](Task* action) {
		action->run(id);
	});
}

int NPC::getVisionRange() const
{
	return visionRange;
}

NPC::stateTypes NPC::getEtat() const
{
	return etat;
}

int NPC::getId() const
{
	return id;
}

NPC::cellType NPC::getPos() const
{
	return pos;
}

NPC::cellType NPC::getTemporaryGoalTile() const
{
	return temporaryGoalTile;
}

NPC::cellType NPC::getTurnDestination() const
{
	return turnDestination;
}

void NPC::setTemporaryGoalTile(const cellType& temporaryGoalTile_)
{
	temporaryGoalTile = temporaryGoalTile_;
}

void NPC::setTurnDestination(const cellType& turnDestination_)
{
	turnDestination = turnDestination_;
}

void NPC::setEtat(stateTypes stateType)
{
	etat = stateType;
}

void NPC::giveGoal(const cellType& temporaryGoalTile_)
{
	setTemporaryGoalTile(temporaryGoalTile_);
	hasGoal = true;
}

void NPC::removeGoal()
{
	hasGoal = false;
}

bool NPC::getHasGoal() const
{
	return hasGoal;
}

void NPC::setHasGoal(bool hasGoal_)
{
	hasGoal = hasGoal_;
}

void NPC::avance(EHexCellDirection direction)
{
	pos = pos.getNeighborFromDirection(direction);
}

void NPC::debug(Logger& logger) const
{
	std::stringstream ss;
	ss << "Position : \nq : " << pos.q << "\tr : " << pos.r << std::endl;
	ss << "Etat : " << stateNames[etat] << std::endl;
	ss << "Goal Final : q : " << temporaryGoalTile.q << ", r : " << temporaryGoalTile.r << std::endl;
	ss << "turnDestination : q : " << turnDestination.q << ", r : " << turnDestination.r << std::endl;
	logger.Log(ss.str());
	stateMachine->debug(logger);
}