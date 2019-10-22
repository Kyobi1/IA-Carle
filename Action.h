#pragma once

struct Action
{
	enum { EXPLORATION, DEPLACEMENT_CIBLE, EN_ATTENTE, ARRIVE, NON_ASSIGNE } etat;
};