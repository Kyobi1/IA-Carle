#pragma once

class Cell
{

};

class NPC
{
	using cellType = Cell;
	cellType pos;
	enum stateTypes{ EN_DEPLACEMENT, EN_ATTENTE, ARRIVE, NON_ASSIGNE } etat;
public:
	NPC();
	NPC(cellType startPos);
};