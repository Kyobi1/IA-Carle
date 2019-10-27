#pragma once
//#include "Action.h"
#include "Task.h"
#include "Transition.h"
#include <vector>

//class Transition;
class State
{
	std::vector<Task*> actions;
	std::vector<Task*> entryActions;
	std::vector<Task*> exitActions;
	std::vector<Transition> transitions;
public:
	State() = default;
	State(const std::vector<Task*>& actions_, const std::vector<Transition>& transitions_) : actions{actions_}, transitions{transitions_}
	{

	}
	~State() {
		std::for_each(begin(transitions), end(transitions), [](Transition t) { t.freeCondition(); });
	}
	std::vector<Task*> getActions() const { return actions; }
	std::vector<Task*> getEntryActions() const { return entryActions; }
	std::vector<Task*> getExitActions() const { return exitActions; }
	std::vector<Transition> getTransitions() const { return transitions; }

	void setActions(const std::vector<Task*>& actions_) { actions = actions_; }
	void setEntryActions(const std::vector<Task*>& entryActions_) { entryActions = entryActions_; }
	void setExitActions(const std::vector<Task*>& exitActions_) { exitActions = exitActions_; }
	void setTransitions(const std::vector<Transition>& transitions_) { transitions = transitions_; }

	void debug(Logger& logger) const { logger.Log("StateDebug"); }
};