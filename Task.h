#pragma once
#include <vector>
#include "NPCMother.h"
#include "HexCell.h"

struct Task {
	static NPCMother* mother;
	static NPC* user;
	HexCell goalTile;
	HexCell turnDestination;
	virtual bool run() {};
};

struct Selector : public Task {
	std::vector<Task*> childrens;

	template <typename ... T>
	Selector(T&& ... args) {
		childrens.push_back(args...);
	}

	bool run();
};

struct Sequence : public Task {
	std::vector<Task*> childrens;

	template <typename ... T>
	Sequence(T&& ... args) {
		childrens.push_back(args...);
	}

	bool run();
};

struct Decorator : public Task {
	Task* child;
};

struct UntilFail : public Decorator {
	UntilFail(Task* child_) {
		child = child_;
	}

	bool run();
};

struct ContactMotherGoal : Task {
	bool run();
};

struct GoalNotReached : Task {
	bool run();
};

struct GoalNotChanged : Task {
	bool run();
};

struct ContactMotherTurnDestination : Task {
	bool run();
};

struct Act : Task {
	bool run();
};
