#include "Place.h"

Place::Place() {
	this->id = (unsigned int)-1;
	this->pos = Pos(0, 0);
}

Place::Place(unsigned int id, Pos pos, bool verbose) {
	this->id = id;
	this->pos = pos;
	this->verbose = verbose;
}

Place::Place(Place* place) {
	this->id = place->id;
	this->pos = place->pos;
}

Place::~Place() {
	id = 0;
	pos.~Pos();
	tokens.clear();
}

unsigned int Place::getId() {
	return id;
}

void Place::setId(unsigned int id) {
	this->id = id;
}

Pos Place::getPos() {
	return pos;
}

void Place::setPos(Pos pos) {
	this->pos = pos;
}

std::vector<PetriToken*> Place::getTokens() {
	return tokens;
}

bool Place::addToken(PetriToken* token) {
	return Utils::add(tokens, token);
}

PetriToken* Place::delToken(PetriToken* token) {
	return Utils::del(tokens, token);
}

void Place::print() {
	std::cout << "\tPlace " << id << " ";
	std::cout << "at (" << pos.x << ", " << pos.y << ") ";
	std::cout << "has " << tokens.size() << " token" << ((tokens.size() != 1) ? "s" : "");
	if (verbose) {
		std::cout << ": " << std::endl;
		for (unsigned int i = 0; i < tokens.size(); i++)
		{
			std::cout << "\t\tPetriToken " << tokens[i]->getId() << std::endl;
		}
	}
	else {
		std::cout << std::endl;
	}
}

void Place::setVerbose(bool verbose)
{
	this->verbose = verbose;
}
