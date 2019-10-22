#include "Task.h"

bool Selector::run()
{
	for (Task& c : childrens)
		if (c.run())
			return true;
	return false;
}

bool Sequence::run()
{
	for (Task& c : childrens)
		if (!c.run())
			return false;
	return true;
}

bool UntilFail::run()
{
	while (true)
		if (!child.run())
			break;
	return true;
}

bool ContactMotherGoal::run()
{
	//goalTile = mother->getGoalTile(user->getId());
	return true;
}

bool GoalNotReached::run()
{
	/*if (goalTile == user->getPos());
		return false;*/
	return true;
}

bool GoalNotChanged::run()
{
	/*if (!(mother->getGoalTile(user->getId()) == goalTile))
		return false;*/
	return true;
}

bool ContactMotherTurnDestination::run()
{
	//turnDestination = mother->getTurnTile(user->getId());
	return true;
}

bool Act::run()
{
	return false;
}
