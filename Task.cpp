#include "Task.h"
#include "Logger.h"

#include <sstream>



bool Selector::run(int idNPC)
{
	for (Task* c : childrens)
		if (c->run(idNPC))
			return true;
	return false;
}

bool Sequence::run(int idNPC)
{
	for (Task* c : childrens)
		if (!c->run(idNPC))
			return false;
	return true;
}

bool UntilFail::run(int idNPC)
{
	while (true)
		if (!child->run(idNPC))
			break;
	return true;
}

bool ContactMotherGoal::run(int idNPC)
{
	/*Logger log;
	log.Init("../Debug", "debugRun.txt");
	std::stringstream ss;
	ss << user;
	log.Log("user address : " + ss.str());*/
	NPCMother& mother = NPCMother::getInstance();
	NPC& npc = mother.getNPCByID(idNPC);
	npc.setTemporaryGoalTile(mother.getGoalNPC(idNPC));
	return true;
}

bool GoalNotReached::run(int idNPC)
{
	NPCMother& mother = NPCMother::getInstance();
	NPC& npc = mother.getNPCByID(idNPC);
	if (mother.getGoalNPC(idNPC) == npc.getPos())
		return false;
	return true;
}

bool GoalNotChanged::run(int idNPC)
{
	NPCMother& mother = NPCMother::getInstance();
	NPC& npc = mother.getNPCByID(idNPC);
	if (mother.getGoalNPC(idNPC) != npc.getTemporaryGoalTile())
		return false;
	return true;
}

//bool ContactMotherTurnDestination::run()
//{
//	//turnDestination = mother->getTurnTile(user->getId());
//	return true;
//}

bool Act::run(int idNPC)
{
	NPCMother& mother = NPCMother::getInstance();
	NPC& npc = mother.getNPCByID(idNPC);
	mother.setNextTile(idNPC);
	EHexCellDirection direction = npc.getPos().directionTo(npc.getTurnDestination());
	if (direction == EHexCellDirection::CENTER)
		mother.NPCResteSurPlace(idNPC);
	else
		mother.NPCAvance(idNPC, direction);
	
	return true;
}

bool NoAct::run(int idNPC)
{
	NPCMother& mother = NPCMother::getInstance();
	mother.NPCResteSurPlace(idNPC);
	return true;
}

bool ToExp::run(int idNPC)
{
	NPCMother& mother = NPCMother::getInstance();
	NPC& npc = mother.getNPCByID(idNPC);
	npc.setEtat(NPC::EXPLORATION);
	return true;
}

bool ToAtt::run(int idNPC)
{
	NPCMother& mother = NPCMother::getInstance();
	NPC& npc = mother.getNPCByID(idNPC);
	npc.setEtat(NPC::EN_ATTENTE);
	return true;
}

bool ToCib::run(int idNPC)
{
	NPCMother& mother = NPCMother::getInstance();
	NPC& npc = mother.getNPCByID(idNPC);
	npc.setEtat(NPC::DEPLACEMENT_CIBLE);
	return true;
}

bool ToArr::run(int idNPC)
{
	NPCMother& mother = NPCMother::getInstance();
	NPC& npc = mother.getNPCByID(idNPC);
	npc.setEtat(NPC::ARRIVE);
	return true;
}