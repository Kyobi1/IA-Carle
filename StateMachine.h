#pragma once
#include "State.h"

#include <algorithm>

class StateMachine
{
	State* currentState;
	int numNPC = -1;
public:
	StateMachine() = default;
	StateMachine(const StateMachine&) = default;
	
	void setState(State* startState, int numNPC_)
	{
		currentState = startState;
		numNPC = numNPC_;
	}

	std::vector<Task*> update()
	{
		std::vector<Transition> transitions = currentState->getTransitions();

		std::vector<Transition>::const_iterator triggered =
			std::find_if(begin(transitions), end(transitions), [this](const Transition& t) -> bool { return t.isTriggered(numNPC); });

		if (triggered != end(transitions))
		{
			State* targetState = triggered->getTargetState();

			std::vector<Task*> actions = currentState->getExitActions();
			std::vector<Task*> actionsTriggered = triggered->getActions();
			std::vector<Task*> entryActions = targetState->getEntryActions();
			actions.insert(std::end(actions), std::begin(actionsTriggered), std::end(actionsTriggered));
			actions.insert(std::end(actions), std::begin(entryActions), std::end(entryActions));

			currentState = targetState;
			return actions;
		}
		return currentState->getActions();
	}

	State* getCurrentState() const
	{
		return currentState;
	}

	void debug(Logger& logger) const
	{
		std::stringstream ss;
		ss << "Current state : " << std::endl;
		currentState->debug(logger);
		ss << "Nb transition : " << currentState->getTransitions().size() << std::endl;
		logger.Log(ss.str());
	}
};