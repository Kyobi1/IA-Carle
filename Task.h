#pragma once
#include <vector>
#include "NPCMother.h"
#include "HexCell.h"

struct Task {

	virtual bool run(int idNPC) = 0;
};

struct Selector : public Task {

	std::vector<Task*> childrens;
	template <typename ... T>
	Selector(T&& ... args) {
		childrens.push_back(args...);
	}

	bool run(int idNPC);
};

struct Sequence : public Task {
	std::vector<Task*> childrens;

	template <typename T, typename ... Ts>
	void addChild(T&& arg, Ts&& ... args) {
		childrens.push_back(arg);
		addChild(args...);
	}

	void addChild() {}

	template <typename ... T>
	Sequence(T&& ... args) {
		addChild(args...);
	}


	bool run(int idNPC);
};

struct Decorator : public Task {
	Task* child;
};

struct UntilFail : public Decorator {
	UntilFail(Task* child_) {
		child = child_;
	}

	bool run(int idNPC);
};

struct ContactMotherGoal : Task {
	bool run(int idNPC);
};

struct GoalNotReached : Task {
	bool run(int idNPC);
};

struct GoalNotChanged : Task {
	bool run(int idNPC);
};

struct Act : Task {
	bool run(int idNPC);
};