#pragma once
//#include "Action.h"
#include "Task.h"
#include "Condition.h"
#include <vector>

class State;
class Transition
{
	std::vector<Task*> actions;
	State* targetState;
	Condition* condition;

public:
	Transition() : targetState(nullptr), condition(nullptr) {}
	Transition(const std::vector<Task*>& actions_, State* targetState_, Condition* condition_) : actions{actions_}, targetState{targetState_}, condition{condition_} {}
	Transition(const Transition&) = default;

	Transition& operator=(const Transition& t) = default;

	~Transition() {}

	bool isTriggered(int numNPC) const { return condition->test(numNPC); }
	State* getTargetState() const { return targetState; }
	std::vector<Task*> getActions() const { return actions; }
	// Ne pas se servir de l'objet apres avoir utlise cette methode
	void freeCondition() { delete condition; }

	void debug(Logger& logger) const {}
};