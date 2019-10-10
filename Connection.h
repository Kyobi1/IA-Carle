#pragma once
#include "Globals.h"

struct Connection {
	STileInfo originNode;
	STileInfo destinationNode;

	static const int cout = 1;

	Connection() = default;

	Connection(const STileInfo& origin, const STileInfo& destination);

	float getCost() const;
};