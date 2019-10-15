#ifndef CONNECTION_H
#define CONNECTION_H

#include "Globals.h"
#include "HexCell.h"
#include "Logger.h"

struct Connection {
	using costValue = unsigned short;
	static const costValue MAX_COST_VALUE;

	HexCell originNode;
	HexCell destinationNode;
	enum ObjectType{ Wall, Window, Door, PressurePlate, Nothing, Forbidden } object;
	const static std::string objectNames[6];
	costValue cost;

	//Connection() = default;
	Connection(const HexCell& origin, const HexCell& destination, const ObjectType& object_);

	void init(const HexCell& origin, const HexCell& destination, const ObjectType& object);

	void setObjectType(ObjectType objectType);
	costValue getCost() const { return cost; }

	void debug(Logger& logger) const;
};

#endif //CONNECTION_H