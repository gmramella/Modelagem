#include "Connection.h"

Connection::Connection(unsigned int id, Place* place, ARC_DIR direction, unsigned int weight, bool isInhibitor) {
	this->id = id;
	this->place = place;
	this->direction = direction;
	this->weight = weight;
	this->isInhibitor = isInhibitor;
}

Connection::Connection(Connection* connection) {
	this->id = connection->id;
	this->place = connection->place;
	this->direction = connection->direction;
	this->weight = connection->weight;
	this->isInhibitor = connection->isInhibitor;
}

Connection::~Connection() {
	id = 0;
	weight = 0;
	direction = 0;
	isInhibitor = false;
	place = nullptr;
}

unsigned int Connection::getId() {
	return id;
}

void Connection::setId(unsigned int id) {
	this->id = id;
}

unsigned int Connection::getWeight() {
	return weight;
}

void Connection::setWeight(unsigned int weight) {
	this->weight = weight;
}

bool Connection::getIsInhibitor() {
	return isInhibitor;
}

void Connection::setIsInhibitor(bool isInhibitor) {
	this->isInhibitor = isInhibitor;
}

Place* Connection::getPlace() {
	return place;
}

void Connection::setPlace(Place* place) {
	this->place = place;
}

ARC_DIR Connection::getDirection() {
	return direction;
}

void Connection::setDirection(ARC_DIR direction) {
	this->direction = direction;
}

void Connection::print() {
	std::cout << "Connection " << id;
	std::cout << ", which has weight of " << weight;
	std::cout << " and is " << (isInhibitor ? "" : "not ") << "an inhibitor, ";
	if (direction == FROM_PLACE_TO_TRANSITION)
		std::cout << "goes from place " << place->getId() << std::endl;
	else
		std::cout << "goes to place " << place->getId() << std::endl;
}
