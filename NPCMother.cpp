#include "NPCMother.h"
#include <algorithm>
#include <iterator>
#include <sstream>
#include "State.h"

NPCMother::NPCMother(const std::vector<NPC>& NPCs, Graph* map_) : map(map_), enfants(NPCs), solutionFound(false), taskMove(nullptr), taskNoMove(nullptr)
{
	
}

void NPCMother::init(const SInitData& _initData, Graph* map_)
{
	solutionFound = false;
	map = map_;
	// Creation des enfants
	std::for_each(_initData.npcInfoArray + 0, _initData.npcInfoArray + _initData.nbNPCs, [this](SNPCInfo infosNPC) {
		enfants.emplace_back(infosNPC.uid, HexCell(infosNPC.q, infosNPC.r), infosNPC.visionRange);
		ordersChilds.emplace_back();
		canMove.push_back(true);
	});
	createStateMachine();
	nextTurn(0);
}

NPCMother::~NPCMother()
{
	for (int i = 0; i < etats.size(); ++i)
	{
		delete etats[i];
	}
	delete taskMove;
	delete taskNoMove;
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

	taskMove = new Sequence(new ContactMotherGoal(), new Sequence(new GoalNotReached(), new GoalNotChanged(), new Act()));
	taskNoMove = new NoAct();

	std::vector<Task*> actions;
	std::vector<Transition> transitions;

	// EXP
	actions.push_back(taskMove);

	std::vector<Task*> act;
	act.push_back(new ToAtt());
	transitions.push_back(Transition(act, etats[NPC::EN_ATTENTE],
		new AndCondition(
			new NotCondition(
				new ConditionResteAssezDeTemps(this)),
			new NotCondition(
				new ConditionNPCArrive(this)))));
	act.clear();
	act.push_back(new Sequence(new ToCib(), taskMove));
	transitions.push_back(Transition(act, etats[NPC::DEPLACEMENT_CIBLE],
		new AndCondition(
			new ConditionNPCAUneCible(this),
			new ConditionResteAssezDeTemps(this))));
	act.clear();
	act.push_back(new ToArr());
	transitions.push_back(Transition(act, etats[NPC::ARRIVE],
			new ConditionNPCArrive(this)));

	//etats[NPC::EXPLORATION]->setEntryActions(actions);
	etats[NPC::EXPLORATION]->setActions(actions);
	etats[NPC::EXPLORATION]->setTransitions(transitions);

	actions.clear();
	transitions.clear();

	// CIB
	actions.push_back(taskMove);

	act.clear();
	act.push_back(new ToAtt());
	transitions.push_back(Transition(act, etats[NPC::EN_ATTENTE],
		new AndCondition(
			new NotCondition(
				new ConditionResteAssezDeTemps(this)),
			new NotCondition(
				new ConditionNPCArrive(this)))));
	act.clear();
	act.push_back(new Sequence(new ToExp(), taskMove));
	transitions.push_back(Transition(act, etats[NPC::EXPLORATION],
		new AndCondition(
			new AndCondition(
				new NotCondition(
					new ConditionNPCAUneCible(this)),
				new ConditionResteAssezDeTemps(this)),
			new NotCondition(
				new ConditionNPCArrive(this)))));
	act.clear();
	act.push_back(new Sequence(new ToArr(), taskNoMove));
	transitions.push_back(Transition(act, etats[NPC::ARRIVE],
			new ConditionNPCArrive(this)));

	//etats[NPC::DEPLACEMENT_CIBLE]->setEntryActions(actions);
	etats[NPC::DEPLACEMENT_CIBLE]->setActions(actions);
	etats[NPC::DEPLACEMENT_CIBLE]->setTransitions(transitions);

	actions.clear();
	transitions.clear();

	// ATT
	actions.push_back(taskNoMove);

	act.clear();
	act.push_back(new Sequence(new ToExp(), taskMove));
	transitions.push_back(Transition(act, etats[NPC::EXPLORATION],
		new AndCondition(
			new AndCondition(
				new NotCondition(
					new ConditionNPCAUneCible(this)),
				new ConditionResteAssezDeTemps(this)),
			new NotCondition(
				new ConditionNPCArrive(this)))));
	act.clear();
	act.push_back(new Sequence(new ToCib(), taskMove));
	transitions.push_back(Transition(act, etats[NPC::DEPLACEMENT_CIBLE],
		new AndCondition(
			new AndCondition(
				new ConditionNPCAUneCible(this),
				new ConditionResteAssezDeTemps(this)),
			new NotCondition(
				new ConditionNPCArrive(this)))));
	act.clear();
	act.push_back(new Sequence(new ToArr(), taskNoMove));
	transitions.push_back(Transition(act, etats[NPC::ARRIVE],
		new ConditionNPCArrive(this)));

	//etats[NPC::EN_ATTENTE]->setEntryActions(actions);
	etats[NPC::EN_ATTENTE]->setActions(actions);
	etats[NPC::EN_ATTENTE]->setTransitions(transitions);

	actions.clear();
	transitions.clear();

	// ARR
	actions.push_back(taskNoMove);

	act.clear();
	act.push_back(new Sequence(new ToCib(), taskMove));
	transitions.push_back(Transition(act, etats[NPC::DEPLACEMENT_CIBLE],
		new ConditionEchangeGoalNPC(this)));

	//etats[NPC::ARRIVE]->setEntryActions(actions);
	etats[NPC::ARRIVE]->setActions(actions);
	etats[NPC::ARRIVE]->setTransitions(transitions);

	actions.clear();
	transitions.clear();

	// NA

	act.clear();
	act.push_back(new ToAtt());
	transitions.push_back(Transition(act, etats[NPC::EN_ATTENTE],
		new AndCondition(
			new NotCondition(
				new ConditionResteAssezDeTemps(this)),
			new NotCondition(
				new ConditionNPCArrive(this)))));
	act.clear();
	act.push_back(new ToExp());
	transitions.push_back(Transition(act, etats[NPC::EXPLORATION],
		new AndCondition(
			new AndCondition(
				new NotCondition(
					new ConditionNPCAUneCible(this)),
				new ConditionResteAssezDeTemps(this)),
			new NotCondition(
				new ConditionNPCArrive(this)))));
	act.clear();
	act.push_back(new ToCib());
	transitions.push_back(Transition(act, etats[NPC::DEPLACEMENT_CIBLE],
		new AndCondition(
			new AndCondition(
				new ConditionNPCAUneCible(this),
				new ConditionResteAssezDeTemps(this)),
			new NotCondition(
				new ConditionNPCArrive(this)))));
	act.clear();
	act.push_back(new Sequence(new ToArr(), taskNoMove));
	transitions.push_back(Transition(act, etats[NPC::ARRIVE],
		new ConditionNPCArrive(this)));

	//etats[NPC::NON_ASSIGNE]->setEntryActions(actions);
	etats[NPC::NON_ASSIGNE]->setActions(actions);
	etats[NPC::NON_ASSIGNE]->setTransitions(transitions);
	
	for (int indexNPC = 0; indexNPC < enfants.size(); ++indexNPC)
	{
		enfants[indexNPC].initStateMachine(etats[NPC::NON_ASSIGNE], indexNPC);
	}
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
	return pathsNPC[getIndexInSolutionNPCFromId(idNPC)].second.front();
}

HexCell NPCMother::getNextTile(int idNPC) const
{
	// TO DO : gestion acces concurrents
	return pathsNPC[getIndexInSolutionNPCFromId(idNPC)].second.back();
}

bool NPCMother::resteAssezDeTemps(int numNPC) const
{
	return true;
	//return tabAssezDeTemps[num];
}

bool NPCMother::NPCSTousArrives()const
{
	//std::all_of
	// A modifier
	std::vector<NPC>::const_iterator it = std::find_if(begin(enfants), end(enfants), [](NPC child)->bool {return child.getEtat() != NPC::ARRIVE; });
	if (it == enfants.end())
	{
		return true;
	}
	return false;
	//return tabNPCSTousArrives[num];
}

bool NPCMother::NPCAUneCible(int numNPC) const
{
	return enfants[numNPC].getHasGoal();
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

bool NPCMother::NPCArrive(int numNPC) const
{
	return solutionFound && NPCCibleAtteinte(numNPC);
}

void NPCMother::nextTurn(int turnNb)
{
	if (!solutionFound)
	{
		std::vector<std::vector<PathFinder::path>> chemins;
		std::vector<int> indexToID;
		std::vector<HexCell> goalsDiscovered;
		goalsDiscovered = map->getDiscoveredGoals();
		// Cree toutes les combinaisons NPC/Chemins vers objectifs
		std::for_each(begin(enfants), end(enfants), [this, &chemins, &indexToID, &goalsDiscovered](const NPC& npc) {
			chemins.push_back(std::vector<PathFinder::path>());
			indexToID.push_back(npc.getId());
			std::for_each(begin(goalsDiscovered), end(goalsDiscovered), [this, &npc, &chemins, &indexToID](HexCell goal) {
				PathFinder::path chemin = map->getPath(npc.getPos(), goal);
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
					break;
				}
				// Selection du chemin le plus court
				PathFinder::path minPath = *std::min_element((*NPCMinPath).begin(), (*NPCMinPath).end(), [](const PathFinder::path& chemin1, const PathFinder::path& chemin2) {
					return chemin1.size() < chemin2.size();
				});
				int offset = NPCMinPath - chemins.begin();
				repartition.emplace_back(indexToID[offset], minPath);
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
			repartition.clear();
			for (int i = 0; i < enfants.size(); ++i)
			{
				int id = enfants[i].getId();
				HexCell tempGoal = map->getHighestUtilityCell(enfants[i].getPos(), enfants[i].getVisionRange());
				
				PathFinder::path chemin = map->getPath(enfants[i].getPos(), tempGoal);
				if (chemin.size() == 0 && tempGoal != enfants[i].getPos()) {
					map->getNode(tempGoal)->pathFinder->updateNodes(*map);
					chemin = map->getPath(enfants[i].getPos(), tempGoal);
				}
				chemin.pop_back();
				repartition.emplace_back(id, chemin);
			}
			
		}
		else
		{

			for (int i = 0; i < enfants.size(); ++i)
				enfants[i].setHasGoal(true);
			solutionFound = true;
		}
		pathsNPC = repartition;
	}

	

	for (int i = 0; i < enfants.size(); ++i)
	{
		canMove[i] = true;
	}

	solveConflicts();

	for (int i = 0; i < enfants.size(); ++i)
	{
		enfants[i].updateStateMachine();
	}
}

void NPCMother::solveConflicts()
{
	std::vector<std::pair<int, HexCell>> tiles;
	std::for_each(begin(pathsNPC), end(pathsNPC), [&tiles](const std::pair<int, PathFinder::path>& chemin) {
		if (chemin.second.size() > 0)
			tiles.emplace_back(chemin.first, chemin.second.back());
	});
	for (int i = 0; i < tiles.size() - 1; ++i)
	{
		for (int j = i + 1; j < tiles.size(); ++j)
		{
			if (tiles[i].second == tiles[j].second)
			{
				if (pathsNPC[getIndexInSolutionNPCFromId(tiles[i].first)].second.size() > pathsNPC[getIndexInSolutionNPCFromId(tiles[j].first)].second.size())
					canMove[getIndexNPCFromId(tiles[j].first)] = false;
				else
					canMove[getIndexNPCFromId(tiles[i].first)] = false;
			}
		}
	}
}

void NPCMother::NPCAvance(int idNPC, EHexCellDirection direction)
{
	int indexNPC = getIndexNPCFromId(idNPC);
	ordersChilds[indexNPC].npcUID = idNPC;
	ordersChilds[indexNPC].orderType = EOrderType::Move;
	if (!canMove[indexNPC])
		direction = EHexCellDirection::CENTER;
	else if (map->getNode(getNPCByID(idNPC).getPos())->connections[direction].object != Connection::Nothing)
	{
		direction = EHexCellDirection::CENTER;
		map->getNode(getNPCByID(idNPC).getTemporaryGoalTile())->pathFinder->updateNodes(*map);
		enfants[indexNPC].setHasGoal(false);
		solutionFound = false;
	}
	ordersChilds[indexNPC].direction = direction;
	enfants[indexNPC].avance(ordersChilds[indexNPC].direction);
	if (ordersChilds[indexNPC].direction != EHexCellDirection::CENTER)
		pathsNPC[getIndexInSolutionNPCFromId(idNPC)].second.pop_back();
}

void NPCMother::NPCResteSurPlace(int idNPC)
{
	int indexNPC = getIndexNPCFromId(idNPC);
	ordersChilds[indexNPC].npcUID = idNPC;
	ordersChilds[indexNPC].orderType = EOrderType::Move;
	ordersChilds[indexNPC].direction = EHexCellDirection::CENTER;
}

void NPCMother::setGoalNPC(int idNPC)
{
	enfants[getIndexNPCFromId(idNPC)].setTemporaryGoalTile(pathsNPC[getIndexInSolutionNPCFromId(idNPC)].second.front());
}

void NPCMother::setNextTile(int idNPC)
{
	enfants[getIndexNPCFromId(idNPC)].setTurnDestination(pathsNPC[getIndexInSolutionNPCFromId(idNPC)].second.back());
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
	return std::find_if(begin(pathsNPC), end(pathsNPC), [idNPC](const std::pair<int, PathFinder::path>& elem)->bool { return elem.first == idNPC;}) - pathsNPC.begin();
}

void NPCMother::debug(Logger& logger) const
{
	std::for_each(begin(enfants), end(enfants), [this, &logger](const NPC& enfant) {
		logger.Log("NPC : ");
		enfant.debug(logger);
	});
}