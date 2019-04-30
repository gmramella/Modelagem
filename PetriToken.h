#pragma once

class PetriToken
{
public:
	PetriToken(unsigned int id);
	PetriToken(PetriToken* petriPetriToken);
	~PetriToken();
	unsigned int getId();
private:
	unsigned int id;
};
