#include "Connection.h"

Connection::Connection(const HexCell& origin, const HexCell& destination, const EObjectType& object_) : originNode{ origin }, destinationNode{ destination }, object{ object_ } { }

void Connection::init(const HexCell& origin, const HexCell& destination, const EObjectType& object_) {
	originNode = origin;
	destinationNode = destination;
	object = object_;
}