#include "NPC.h"
#include "NPCMother.h"
#include "StateMachine.h"
#include "Task.h"


const std::string NPC::stateNames[5] = { "EXPLORATION", "DEPLACEMENT_CIBLE", "EN_ATTENTE", "ARRIVE", "NON_ASSIGNE" };

NPC::NPC() : id(-1), pos(), etat(NON_ASSIGNE), hasFinalGoal(false)
{
	stateMachine = new StateMachine();
	taskMove = new Sequence(new ContactMotherGoal(), new Sequence(new GoalNotReached(), new GoalNotChanged(), new Act()));
}

NPC::NPC(int id_, cellType startPos, int visionRange_, stateTypes etat_) : id(id_), pos(startPos), visionRange(visionRange_), etat(etat_), hasFinalGoal(false)
{
	stateMachine = new StateMachine();
	taskMove = new Sequence(new ContactMotherGoal(), new Sequence(new GoalNotReached(), new GoalNotChanged(), new Act()));
}

NPC::NPC(const NPC& npc_) : pos{ npc_.pos }, etat{ npc_.etat }, id{ npc_.id }, visionRange{ npc_.visionRange }, hasFinalGoal{ false }
{
	stateMachine = new StateMachine(*(npc_.stateMachine));
	taskMove = new Sequence(new ContactMotherGoal(), new Sequence(new GoalNotReached(), new GoalNotChanged(), new Act()));
}

NPC::~NPC()
{
	delete taskMove;
	delete stateMachine;
}

void NPC::initStateMachine(State* startState, int numNPC)
{
	logger.Init("../Debug", "debugNPC.txt");
	stateMachine->setState(startState, numNPC);
}

void NPC::updateStateMachine()
{
	std::vector<Action> actions = stateMachine->update();
	logger.Log("actions.size() : " + std::to_string(actions.size()));
	logger.Log("action : " + std::to_string(actions[0].etat));
	std::for_each(begin(actions), end(actions), [this](Action action) {
		switch (action.etat)
		{
		case Action::EXPLORATION:
			this->etat = EXPLORATION;
			taskMove->run(id);
			break;
		case Action::DEPLACEMENT_CIBLE:
			this->etat = DEPLACEMENT_CIBLE;
			taskMove->run(id);
			break;
		case Action::EN_ATTENTE:
			this->etat = EN_ATTENTE;
			break;
		case Action::ARRIVE:
			this->etat = ARRIVE;
			break;
		case Action::NON_ASSIGNE:
			this->etat = NON_ASSIGNE;
			break;
		default:
			
			break;
		}
	});
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

void NPC::giveFinalGoal(const cellType& temporaryGoalTile_)
{
	setTemporaryGoalTile(temporaryGoalTile_);
	hasFinalGoal = true;
}

void NPC::removeFinalGoal()
{
	hasFinalGoal = false;
}

bool NPC::getHasFinalGoal() const
{
	return hasFinalGoal;
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