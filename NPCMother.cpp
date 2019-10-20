#include "NPCMother.h"
#include <algorithm>
#include <iterator>
#include "State.h"

NPCMother::NPCMother(const std::vector<NPC>& NPCs, Graph* map_) : map(map_), enfants(NPCs)
{
	
}

void NPCMother::init(const SInitData& _initData, Graph* map_)
{
	map = map_;
	std::for_each(_initData.npcInfoArray + 0, _initData.npcInfoArray + _initData.nbNPCs, [this](SNPCInfo infosNPC) {
		enfants.push_back(NPC(infosNPC.uid, HexCell(infosNPC.q, infosNPC.r), infosNPC.visionRange));
	});
	createStateMachine();
}

NPCMother::~NPCMother()
{
	for (int i = 0; i < etats.size(); ++i)
	{
		delete etats[i];
	}
}

void NPCMother::createStateMachine()
{
	State* EXP = new State();
	State* CIB = new State();
	State* ATT = new State();
	State* ARR = new State();
	State* NA = new State();
	etats.push_back(EXP);
	etats.push_back(CIB);
	etats.push_back(ATT);
	etats.push_back(ARR);
	etats.push_back(NA);

	std::vector<Action> actions;
	std::vector<Transition> transitions;

	// EXP
	Action action;
	//action.type = Action::MOVE;
	actions.push_back(action);

	std::vector<Action> act;
	Action actionTransition;
	actionTransition.etat = Action::EN_ATTENTE;
	act.push_back(actionTransition);
	transitions.push_back(Transition(act, etats[NPC::EN_ATTENTE],
		new AndCondition(
			new NotCondition(
				new ConditionResteAssezDeTemps(this)),
			new NotCondition(
				new ConditionNPCSArrives(this)))));
	act.clear();
	actionTransition.etat = Action::DEPLACEMENT_CIBLE;
	act.push_back(actionTransition);
	transitions.push_back(Transition(act, etats[NPC::DEPLACEMENT_CIBLE],
		new AndCondition(
		new AndCondition(
			new ConditionNPCAUneCible(this),
			new ConditionResteAssezDeTemps(this)),
		new NotCondition(
			new ConditionNPCSArrives(this)))));
	act.clear();
	actionTransition.etat = Action::ARRIVE;
	act.push_back(actionTransition);
	transitions.push_back(Transition(act, etats[NPC::ARRIVE],
		new ConditionNPCSArrives(this)));

	etats[NPC::EXPLORATION]->setActions(actions);
	etats[NPC::EXPLORATION]->setTransitions(transitions);

	actions.clear();
	transitions.clear();

	// CIB
	//Action action;
	//action.type = Action::MOVE;
	actions.push_back(action);

	//std::vector<Action> act;
	act.clear();
	actionTransition.etat = Action::EN_ATTENTE;
	act.push_back(actionTransition);
	transitions.push_back(Transition(act, etats[NPC::EN_ATTENTE],
		new AndCondition(
			new NotCondition(
				new ConditionResteAssezDeTemps(this)),
			new NotCondition(
				new ConditionNPCSArrives(this)))));
	act.clear();
	actionTransition.etat = Action::EXPLORATION;
	act.push_back(actionTransition);
	transitions.push_back(Transition(act, etats[NPC::EXPLORATION],
		new AndCondition(
			new AndCondition(
				new NotCondition(
					new ConditionNPCAUneCible(this)),
				new ConditionResteAssezDeTemps(this)),
			new NotCondition(
				new ConditionNPCSArrives(this)))));
	act.clear();
	actionTransition.etat = Action::ARRIVE;
	act.push_back(actionTransition);
	transitions.push_back(Transition(act, etats[NPC::ARRIVE],
		new ConditionNPCSArrives(this)));

	etats[NPC::DEPLACEMENT_CIBLE]->setActions(actions);
	etats[NPC::DEPLACEMENT_CIBLE]->setTransitions(transitions);

	actions.clear();
	transitions.clear();

	// ATT
	//Action action;
	//action.type = Action::MOVE;
	actions.push_back(action);

	//std::vector<Action> act;
	act.clear();
	actionTransition.etat = Action::EXPLORATION;
	act.push_back(actionTransition);
	transitions.push_back(Transition(act, etats[NPC::EXPLORATION],
		new AndCondition(
			new AndCondition(
				new NotCondition(
					new ConditionNPCAUneCible(this)),
				new ConditionResteAssezDeTemps(this)),
			new NotCondition(
				new ConditionNPCSArrives(this)))));
	act.clear();
	actionTransition.etat = Action::DEPLACEMENT_CIBLE;
	act.push_back(actionTransition);
	transitions.push_back(Transition(act, etats[NPC::DEPLACEMENT_CIBLE],
		new AndCondition(
			new AndCondition(
				new ConditionNPCAUneCible(this),
				new ConditionResteAssezDeTemps(this)),
			new NotCondition(
				new ConditionNPCSArrives(this)))));
	act.clear();
	actionTransition.etat = Action::ARRIVE;
	act.push_back(actionTransition);
	transitions.push_back(Transition(act, etats[NPC::ARRIVE],
		new ConditionNPCSArrives(this)));

	etats[NPC::EN_ATTENTE]->setActions(actions);
	etats[NPC::EN_ATTENTE]->setTransitions(transitions);

	actions.clear();
	transitions.clear();

	// ARR
	//Action action;
	//action.type = Action::MOVE;
	actions.push_back(action);

	//std::vector<Action> act;
	act.clear();
	actionTransition.etat = Action::DEPLACEMENT_CIBLE;
	act.push_back(actionTransition);
	transitions.push_back(Transition(act, etats[NPC::DEPLACEMENT_CIBLE],
		new ConditionEchangeGoalNPC(this)));

	etats[NPC::ARRIVE]->setActions(actions);
	etats[NPC::ARRIVE]->setTransitions(transitions);

	actions.clear();
	transitions.clear();

	// NA
	//Action action;
	//action.type = Action::MOVE;
	actions.push_back(action);

	//std::vector<Action> act;
	act.clear();
	actionTransition.etat = Action::EN_ATTENTE;
	act.push_back(actionTransition);
	transitions.push_back(Transition(act, etats[NPC::EN_ATTENTE],
		new AndCondition(
			new NotCondition(
				new ConditionResteAssezDeTemps(this)),
			new NotCondition(
				new ConditionNPCSArrives(this)))));
	act.clear();
	actionTransition.etat = Action::EXPLORATION;
	act.push_back(actionTransition);
	transitions.push_back(Transition(act, etats[NPC::EXPLORATION],
		new AndCondition(
			new AndCondition(
				new NotCondition(
					new ConditionNPCAUneCible(this)),
				new ConditionResteAssezDeTemps(this)),
			new NotCondition(
				new ConditionNPCSArrives(this)))));
	act.clear();
	actionTransition.etat = Action::DEPLACEMENT_CIBLE;
	act.push_back(actionTransition);
	transitions.push_back(Transition(act, etats[NPC::DEPLACEMENT_CIBLE],
		new AndCondition(
			new AndCondition(
				new ConditionNPCAUneCible(this),
				new ConditionResteAssezDeTemps(this)),
			new NotCondition(
				new ConditionNPCSArrives(this)))));
	act.clear();
	actionTransition.etat = Action::ARRIVE;
	act.push_back(actionTransition);
	transitions.push_back(Transition(act, etats[NPC::ARRIVE],
		new ConditionNPCSArrives(this)));

	etats[NPC::NON_ASSIGNE]->setActions(actions);
	etats[NPC::NON_ASSIGNE]->setTransitions(transitions);
	
	for (int indexNPC = 0; indexNPC < enfants.size(); ++indexNPC)
	{
		enfants[indexNPC].initStateMachine(etats[NPC::NON_ASSIGNE], indexNPC);
	}
}

bool NPCMother::resteAssezDeTemps(int numNPC) const
{
	return tabAssezDeTemps[num];
}

bool NPCMother::NPCSTousArrives() const
{
	return tabNPCSTousArrives[num];
}

bool NPCMother::NPCAUneCible(int numNPC) const
{
	return tabNPCAUneCible[num];
}

bool NPCMother::NPCCibleAtteinte(int numNPC) const
{
	return tabNPCCibleAtteinte[num];
}

bool NPCMother::NPCEchangeCible(int numNPC) const
{
	return tabNPCEchangeCible[num];
}

void NPCMother::nextTurn()
{
	for (int i = 0; i < enfants.size(); ++i)
	{
		enfants[i].updateStateMachine();
	}
	++num;
}

void NPCMother::debug(Logger& logger) const
{
	std::for_each(begin(enfants), end(enfants), [this, &logger](NPC enfant) {
		logger.Log("NPC : ");
		enfant.debug(logger);
	});
}