#include "PetriToken.h"

PetriToken::PetriToken(unsigned int id) {
	this->id = id;
}

PetriToken::PetriToken(PetriToken* petriPetriToken) {
	this->id = petriPetriToken->id;
}

PetriToken::~PetriToken() {
	id = 0;
}

unsigned int PetriToken::getId()
{
	return id;
}
