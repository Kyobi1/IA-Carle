#pragma once
#include "HexCell.h"

class NPC
{
	using cellType = HexCell;
	cellType pos;
	enum stateTypes{ EN_DEPLACEMENT, EN_ATTENTE, ARRIVE, NON_ASSIGNE } etat;
public:
	NPC();
	NPC(cellType startPos);
};