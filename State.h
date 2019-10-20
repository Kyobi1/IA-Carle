#pragma once
#include "Action.h"
#include "Transition.h"
#include <vector>

//class Transition;
class State
{
	std::vector<Action> actions;
	std::vector<Action> entryActions;
	std::vector<Action> exitActions;
	std::vector<Transition> transitions;
public:
	State() = default;
	State(const std::vector<Action>& actions_, const std::vector<Transition>& transitions_) : actions{actions_}, transitions{transitions_}
	{

	}
	~State() { std::for_each(begin(transitions), end(transitions), [](Transition t) { t.freeCondition(); }); }
	std::vector<Action> getActions() const { return actions; }
	std::vector<Action> getEntryActions() const { return entryActions; }
	std::vector<Action> getExitActions() const { return exitActions; }
	std::vector<Transition> getTransitions() const { return transitions; }

	void setActions(const std::vector<Action>& actions_) { actions = actions_; }
	void setEntryActions(const std::vector<Action>& entryActions_) { entryActions = entryActions_; }
	void setExitActions(const std::vector<Action>& exitActions_) { exitActions = exitActions_; }
	void setTransitions(const std::vector<Transition>& transitions_) { transitions = transitions_; }

	void debug(Logger& logger) const { logger.Log("StateDebug"); }
};