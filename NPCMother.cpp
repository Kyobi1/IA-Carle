#include "NPCMother.h"
#include <algorithm>
#include <iterator>
#include <sstream>
#include "State.h"

NPCMother::NPCMother(const std::vector<NPC>& NPCs, Graph* map_) : map(map_), enfants(NPCs), solutionFound(false)
{
	
}

void NPCMother::init(const SInitData& _initData, Graph* map_)
{
	logger.Init("../Debug", "debug.txt");
	solutionFound = false;
	map = map_;
	std::for_each(_initData.npcInfoArray + 0, _initData.npcInfoArray + _initData.nbNPCs, [this](SNPCInfo infosNPC) {
		enfants.push_back(NPC(infosNPC.uid, HexCell(infosNPC.q, infosNPC.r), infosNPC.visionRange));
		ordersChilds.emplace_back();
	});
	createStateMachine();
	initGoals();
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
	action.etat = Action::EXPLORATION;
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
	action.etat = Action::DEPLACEMENT_CIBLE;
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
	action.etat = Action::EN_ATTENTE;
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
	action.etat = Action::ARRIVE;
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
	action.etat = Action::NON_ASSIGNE;
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

void NPCMother::initGoals()
{
	const std::unordered_map<Graph::graphKey, Node>& mapGraph = map->getNodes();
	std::for_each(begin(mapGraph), end(mapGraph), [this](const std::pair<const Graph::graphKey, Node>& node) {
		if (node.second.nodeInfos.type == EHexCellType::Goal)
			goalsDiscovered.push_back(node.first);
	});
}

void NPCMother::takeDecisons()
{
	
}

void NPCMother::giveOrders(std::list<SOrder>& _orders)
{
	std::for_each(begin(ordersChilds), end(ordersChilds), [&_orders](SOrder order) { _orders.push_back(order); });
}

HexCell NPCMother::getGoalNPC(int idNPC) const
{
	return solution[getIndexInSolutionNPCFromId(idNPC)].second.front();
}

HexCell NPCMother::getNextTile(int idNPC) const
{
	// TO DO : gestion acces concurrents
	return solution[getIndexInSolutionNPCFromId(idNPC)].second.back();
}

bool NPCMother::resteAssezDeTemps(int numNPC) const
{
	return true;
	//return tabAssezDeTemps[num];
}

bool NPCMother::NPCSTousArrives() const
{
	//std::all_of
	std::vector<NPC>::const_iterator it = std::find_if(begin(enfants), end(enfants), [](NPC child)->bool {return child.getEtat() != NPC::ARRIVE; });
	if (it == enfants.end())
		return true;
	return false;
	//return tabNPCSTousArrives[num];
}

bool NPCMother::NPCAUneCible(int numNPC) const
{
	return enfants[numNPC].getHasFinalGoal();
	//return tabNPCAUneCible[num];
}

bool NPCMother::NPCCibleAtteinte(int numNPC) const
{
	return enfants[numNPC].getPos() == enfants[numNPC].getTemporaryGoalTile();
	//return tabNPCCibleAtteinte[num];
}

bool NPCMother::NPCEchangeCible(int numNPC) const
{
	return false;
	//return tabNPCEchangeCible[num];
}

void NPCMother::nextTurn()
{
	if (!solutionFound)
	{
		std::vector<std::vector<PathFinder::path>> chemins;
		std::vector<int> indexToID;
		std::for_each(begin(enfants), end(enfants), [this, &chemins, &indexToID](const NPC& npc) {
			chemins.push_back(std::vector<PathFinder::path>());
			indexToID.push_back(npc.getId());
			std::for_each(begin(goalsDiscovered), end(goalsDiscovered), [this, &npc, &chemins, &indexToID](HexCell goal) {
				PathFinder::path chemin = map->getPath(goal, npc.getPos());
				if (chemin.size() > 0)
				{
					chemin.pop_back();
					chemins[chemins.size() - 1].push_back(chemin);
				}
			});
		});

		std::vector<std::pair<int, PathFinder::path>> repartition;

		bool noSolution = true;
		if (goalsDiscovered.size() >= enfants.size())
		{
			while (chemins.size() > 0)
			{
				// NPC ayant le moins de chemins possibles
				auto NPCMinPath = std::min_element(chemins.begin(), chemins.end(), [](const std::vector<PathFinder::path>& chemin1, const std::vector<PathFinder::path>& chemin2) {
					return chemin1.size() < chemin2.size();
				});
				// Si le NPC n'a pas de chemin -> pas de solution
				if (NPCMinPath->size() == 0)
				{
					noSolution = true;
					/*int offset = NPCMinPath - chemins.begin();
					indexToID.erase(indexToID.begin() + offset);
					chemins.erase(NPCMinPath);*/
					break;
				}
				// Selection du chemin le plus court
				PathFinder::path minPath = *std::min_element((*NPCMinPath).begin(), (*NPCMinPath).end(), [](const PathFinder::path& chemin1, const PathFinder::path& chemin2) {
					return chemin1.size() < chemin2.size();
				});
				int offset = NPCMinPath - chemins.begin();
				repartition.emplace_back(indexToID[offset], minPath);
				//logger.Log("q : " + std::to_string(minPath->front().q) + ", r : " + std::to_string(minPath->front().r));
				indexToID.erase(indexToID.begin() + offset);
				chemins.erase(NPCMinPath);
				for (int i = 0; i < chemins.size(); ++i)
				{
					for (int j = 0; j < chemins[i].size(); ++j)
					{
						if (chemins[i][j].front() == minPath.front())
						{
							chemins[i].erase(chemins[i].begin() + j);
							--j;
						}
					}
				}
			}
			if (chemins.size() == 0)
				noSolution = false;
		}

		if (noSolution)
		{

		}
		else
		{
			solutionFound = true;
			solution = repartition;
			/*std::for_each(begin(solution), end(solution), [this](const std::pair<int, PathFinder::path>& association) {
				enfants[getIndexNPCFromId(association.first)].giveFinalGoal(association.second.back());
			});*/
		}
	}
	logger.Log("enfants.size() : " + std::to_string(enfants.size()));

	for (int i = 0; i < enfants.size(); ++i)
	{
		enfants[i].updateStateMachine();
	}
	//ordersChilds.clear();
	/*for (int i = 0; i < solution.size(); ++i)
	{
		logger.Log("enfant : \tq : " + std::to_string(enfants[getIndexNPCFromId(solution[i].first)].getPos().q) + ", r : " + std::to_string(enfants[getIndexNPCFromId(solution[i].first)].getPos().r));
		logger.Log("solution[i].second.back() : \tq : " + std::to_string(solution[i].second.back().q) + ", r : " + std::to_string(solution[i].second.back().r));
		logger.Log("direction : " + std::to_string(enfants[getIndexNPCFromId(solution[i].first)].getPos().directionTo(solution[i].second.back())));
		NPCAvance(solution[i].first, enfants[getIndexNPCFromId(solution[i].first)].getPos().directionTo(solution[i].second.back()));
		solution[i].second.pop_back();
	}*/
	//++num;
}

void NPCMother::NPCAvance(int idNPC, EHexCellDirection direction)
{
	logger.Log("direction : " + std::to_string(direction));
	int indexNPC = getIndexNPCFromId(idNPC);
	ordersChilds[indexNPC].npcUID = idNPC;
	ordersChilds[indexNPC].orderType = EOrderType::Move;
	ordersChilds[indexNPC].direction = direction;
	enfants[indexNPC].avance(direction);
	if (direction != EHexCellDirection::CENTER)
		solution[getIndexInSolutionNPCFromId(idNPC)].second.pop_back();
}

void NPCMother::setGoalNPC(int idNPC)
{
	enfants[getIndexNPCFromId(idNPC)].setTemporaryGoalTile(solution[getIndexInSolutionNPCFromId(idNPC)].second.front());
}

void NPCMother::setNextTile(int idNPC)
{
	enfants[getIndexNPCFromId(idNPC)].setTurnDestination(solution[getIndexInSolutionNPCFromId(idNPC)].second.back());
}

NPC& NPCMother::getNPCByID(int idNPC)
{
	return enfants[getIndexNPCFromId(idNPC)];
}

// Attention si id n'existe pas
int NPCMother::getIndexNPCFromId(int idNPC) const
{
	return std::find_if(begin(enfants), end(enfants), [idNPC](const NPC& enfant)->bool { return enfant.getId() == idNPC; }) - enfants.begin();
}

int NPCMother::getIndexInSolutionNPCFromId(int idNPC) const
{
	return std::find_if(begin(solution), end(solution), [idNPC](const std::pair<int, PathFinder::path>& elem)->bool { return elem.first == idNPC;}) - solution.begin();
}

void NPCMother::debug(Logger& logger) const
{
	std::for_each(begin(enfants), end(enfants), [this, &logger](const NPC& enfant) {
		logger.Log("NPC : ");
		enfant.debug(logger);
	});
}