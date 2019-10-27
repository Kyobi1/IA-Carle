#pragma once
#include <vector>
#include "NPCMother.h"
#include "HexCell.h"

struct Task {

	virtual bool run(int idNPC) = 0;
	virtual ~Task() {}
};

struct Selector : public Task {

	std::vector<Task*> childrens;
	template <typename ... T>
	Selector(T&& ... args) {
		childrens.push_back(args...);
	}
	~Selector() { std::for_each(begin(childrens), end(childrens), [](Task* task) { delete task; }); }

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
	~Sequence() { std::for_each(begin(childrens), end(childrens), [](Task* task) { delete task; }); }

	bool run(int idNPC);
};

struct Decorator : public Task {
	Task* child;
	~Decorator() { delete child; }
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

struct NoAct : Task {
	bool run(int idNPC);
};

struct ToExp : Task {
	bool run(int idNPC);
};

struct ToAtt : Task {
	bool run(int idNPC);
};

struct ToCib : Task {
	bool run(int idNPC);
};

struct ToArr : Task {
	bool run(int idNPC);
};