#ifndef CONNECTION_H
#define CONNECTION_H

#include "Globals.h"
#include "HexCell.h"

struct Connection {
	HexCell originNode;
	HexCell destinationNode;
	EObjectType object;

	Connection() = default;
	void init(const HexCell& origin, const HexCell& destination);

	Connection(const HexCell& origin, const HexCell& destination);

	float getCost() const { return 1.0f; }
};

#endif //CONNECTION_H