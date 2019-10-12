#ifndef CONNECTION_H
#define CONNECTION_H

#include "Globals.h"
#include "HexCell.h"
#include "Logger.h"

struct Connection {
	HexCell originNode;
	HexCell destinationNode;
	enum ObjectType{ Wall, Window, Door, PressurePlate, Nothing, Forbidden } object;
	const static std::string objectNames[6];

	Connection() = default;
	Connection(const HexCell& origin, const HexCell& destination, const ObjectType& object_);

	void init(const HexCell& origin, const HexCell& destination, const ObjectType& object);

	float getCost() const { return 1.0f; }

	void debug(Logger& logger) const;
};

#endif //CONNECTION_H