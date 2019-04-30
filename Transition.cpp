#include "Transition.h"

Transition::Transition(unsigned int id, bool isEnabled, bool verbose) {
	this->id = id;
	this->isEnabled = isEnabled;
	this->verbose = verbose;
}

Transition::Transition(Transition* transition) {
	this->id = transition->id;
	this->isEnabled = transition->isEnabled;
}

Transition::~Transition() {
	id = 0; isEnabled = false;
	inputConnections.clear();
	outputConnections.clear();
}

unsigned int Transition::getId() {
	return id;
}

void Transition::setId(unsigned int id) {
	this->id = id;
}

bool Transition::getIsEnabled() {
	return isEnabled;
}

void Transition::setIsEnabled(bool isEnabled) {
	this->isEnabled = isEnabled;
}

std::vector<Connection*> Transition::getInputConnections() {
	return inputConnections;
}

bool Transition::addInputConnection(Connection* connection) {
	if (connection->getDirection() == FROM_PLACE_TO_TRANSITION) return Utils::add(inputConnections, connection);
	else return false;
}

Connection* Transition::delInputConnection(Connection* connection) {
	return Utils::del(inputConnections, connection);
}

std::vector<Connection*> Transition::getOutputConnections() {
	return outputConnections;
}

bool Transition::addOutputConnection(Connection* connection) {
	/*int sumInputWeights = 0;
	for (std::vector<Connection*>::iterator it = inputConnections.begin(); it != inputConnections.end(); it++) {
	sumInputWeights += (*it)->getWeight();
	}
	int sumOutputWeights = connection->getWeight();
	for (std::vector<Connection*>::iterator it = outputConnections.begin(); it != outputConnections.end(); it++) {
	sumOutputWeights += (*it)->getWeight();
	}
	if (sumOutputWeights <= sumInputWeights) {
	if (connection->getDirection() == FROM_TRANSITION_TO_PLACE) return Utils::add(outputConnections, connection);
	else return false;
	}
	else {
	std::cout << "The sum of the weights on the output connections cannot be greater than that of the input connections" << std::endl;
	return false;
	}*/
	if (connection->getDirection() == FROM_TRANSITION_TO_PLACE) return Utils::add(outputConnections, connection);
	else return false;
}

Connection* Transition::delOutputConnection(Connection* connection) {
	return Utils::del(outputConnections, connection);
}

void Transition::print() {
	std::cout << "\tTransition " << id << " ";
	std::cout << "has " << inputConnections.size() << " input place" << ((inputConnections.size() != 1) ? "s " : " ");
	std::cout << "and " << outputConnections.size() << " output place" << ((outputConnections.size() != 1) ? "s" : "");
	if (verbose) {
		std::cout << ": " << std::endl;
		for (unsigned int i = 0; i < inputConnections.size(); i++)
		{
			std::cout << "\t\tInput " << inputConnections[i]->getId() << std::endl;
		}
		for (unsigned int i = 0; i < outputConnections.size(); i++)
		{
			std::cout << "\t\tOutput " << outputConnections[i]->getId() << std::endl;
		}
	}
	else {
		std::cout << std::endl;
	}
}

void Transition::setVerbose(bool verbose)
{
	this->verbose = verbose;
}
