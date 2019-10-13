#include "Connection.h"
#include <sstream>

const std::string Connection::objectNames[6] = { "Wall", "Window", "Door", "Pressure Plate", "Nothing", "Forbidden" };

Connection::Connection(const HexCell& origin, const HexCell& destination, const ObjectType& object_) : originNode{ origin }, destinationNode{ destination }, object{ object_ } { }

void Connection::init(const HexCell& origin, const HexCell& destination, const ObjectType& object_) {
	originNode = origin;
	destinationNode = destination;
	object = object_;
}

void Connection::debug(Logger& logger) const
{
	std::stringstream ss;
	ss << "OriginNode : \nq : " << originNode.q << "\tr : " << originNode.r << std::endl;
	ss << "DestinationNode : \nq : " << destinationNode.q << "\tr : " << destinationNode.r << std::endl;
	ss << "Type : " << objectNames[object] << std::endl;
	logger.Log(ss.str());
}