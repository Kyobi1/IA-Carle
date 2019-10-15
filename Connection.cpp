#include "Connection.h"
#include <sstream>

const Connection::costValue Connection::MAX_COST_VALUE = UINT16_MAX;

const std::string Connection::objectNames[6] = { "Wall", "Window", "Door", "Pressure Plate", "Nothing", "Forbidden" };

Connection::Connection(const HexCell& origin, const HexCell& destination, const ObjectType& object_) 
	: originNode{ origin }, destinationNode{ destination }, object{ object_ } 
{ 
	if (object_ != ObjectType::Nothing) cost = MAX_COST_VALUE;
	else cost = 1;
}

void Connection::init(const HexCell& origin, const HexCell& destination, const ObjectType& object_) {
	originNode = origin;
	destinationNode = destination;
	object = object_;
	if (object_ != ObjectType::Nothing) cost = MAX_COST_VALUE;
	else cost = 1;
}

void Connection::setObjectType(ObjectType objectType)
{
	object = objectType;
	if (object != ObjectType::Nothing) cost = MAX_COST_VALUE;
	else cost = 1;
}

void Connection::debug(Logger& logger) const
{
	std::stringstream ss;
	ss << "OriginNode : \nq : " << originNode.q << "\tr : " << originNode.r << std::endl;
	ss << "DestinationNode : \nq : " << destinationNode.q << "\tr : " << destinationNode.r << std::endl;
	ss << "Type : " << objectNames[object] << std::endl;
	logger.Log(ss.str());
}