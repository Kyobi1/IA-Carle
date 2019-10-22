#include "NPC.h"
#include "StateMachine.h"

const std::string NPC::stateNames[5] = { "EXPLORATION", "DEPLACEMENT_CIBLE", "EN_ATTENTE", "ARRIVE", "NON_ASSIGNE" };

NPC::NPC() : id(-1), pos(), etat(NON_ASSIGNE)
{
	stateMachine = new StateMachine();
}

NPC::NPC(int id_, cellType startPos, int visionRange_, stateTypes etat_) : id(id_), pos(startPos), visionRange(visionRange_), etat(etat_)
{
	stateMachine = new StateMachine();
}

NPC::NPC(const NPC& npc_) : pos{npc_.pos}, etat{npc_.etat}, id{npc_.id}, visionRange{npc_.visionRange}
{
	stateMachine = new StateMachine(*(npc_.stateMachine));
}

NPC::~NPC()
{
	delete stateMachine;
}

void NPC::initStateMachine(State* startState, int numNPC)
{
	stateMachine->setState(startState, numNPC);
}

void NPC::updateStateMachine()
{
	std::vector<Action> actions = stateMachine->update();
	std::for_each(begin(actions), end(actions), [this](Action action) {
		switch (action.etat)
		{
		case Action::EXPLORATION:
			this->etat = EXPLORATION;
			break;
		case Action::DEPLACEMENT_CIBLE:
			this->etat = DEPLACEMENT_CIBLE;
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

void NPC::debug(Logger& logger) const
{
	std::stringstream ss;
	ss << "Position : \nq : " << pos.q << "\tr : " << pos.r << std::endl;
	ss << "Etat : " << stateNames[etat] << std::endl;
	logger.Log(ss.str());
	stateMachine->debug(logger);
}