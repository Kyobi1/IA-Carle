#pragma once
#include "NPCMother.h"

class Condition
{
protected:
	NPCMother* mother;
public:
	Condition() : mother(nullptr) {}
	Condition(NPCMother* mother_) : mother(mother_) {}
	virtual ~Condition() = default;
	virtual bool test(int numNPC) const = 0;
};

class ConditionResteAssezDeTemps : public Condition
{
public:
	ConditionResteAssezDeTemps(NPCMother* mother_) : Condition(mother_) {}

	bool test(int numNPC) const
	{
		return mother->resteAssezDeTemps(numNPC);
	}
};

class ConditionNPCSArrives : public Condition
{
public:
	ConditionNPCSArrives(NPCMother* mother_) : Condition(mother_) {}

	bool test(int numNPC) const
	{
		return mother->NPCSTousArrives();
	}
};

class ConditionNPCAUneCible : public Condition
{
public:
	ConditionNPCAUneCible(NPCMother* mother_) : Condition(mother_) {}

	bool test(int numNPC) const
	{
		return mother->NPCAUneCible(numNPC);
	}
};

class ConditionCibleAtteinte : public Condition
{
public:
	ConditionCibleAtteinte(NPCMother* mother_) : Condition(mother_) {}

	bool test(int numNPC) const
	{
		return mother->NPCCibleAtteinte(numNPC);
	}
};

class ConditionEchangeGoalNPC : public Condition
{
public:
	ConditionEchangeGoalNPC(NPCMother* mother_) : Condition(mother_) {}

	bool test(int numNPC) const
	{
		return mother->NPCEchangeCible(numNPC);
	}
};

class NotCondition : public Condition
{
	Condition* a;
public:
	NotCondition(Condition* a_) : a{a_} {}
	~NotCondition() { delete a; }
	bool test(int numNPC) const
	{
		return !a->test(numNPC);
	}
};

class AndCondition : public Condition
{
	Condition* a;
	Condition* b;
public:
	AndCondition(Condition* a_, Condition* b_) : a{ a_ }, b{ b_ } {}
	~AndCondition() { delete a; delete b; }
	bool test(int numNPC) const
	{
		return a->test(numNPC) && b->test(numNPC);
	}
};

class OrCondition : public Condition
{
	Condition* a;
	Condition* b;
public:
	OrCondition(Condition* a_, Condition* b_) : a{ a_ }, b{ b_ } {}
	~OrCondition() { delete a; delete b; }
	bool test(int numNPC) const
	{
		return a->test(numNPC) || b->test(numNPC);
	}
};