#pragma once
#include <vector>
#include <iostream>
#include "PetriToken.h"
#include "Utils.h"

struct Pos {
	int x;
	int y;
	Pos() { x = 0; y = 0; }
	Pos(int x, int y) { this->x = x; this->y = y; }
	~Pos() { x = 0; y = 0; }
	bool operator == (const Pos& other) { return (this->x == other.x) && (this->y == other.y); }
};

class Place
{
public:
	Place();
	Place(unsigned int id, Pos pos, bool verbose = false);
	Place(Place* place);
	~Place();
	unsigned int getId();
	void setId(unsigned int id);
	Pos getPos();
	void setPos(Pos pos);
	std::vector<PetriToken*> getTokens();
	bool addToken(PetriToken* token);
	PetriToken* delToken(PetriToken* token);
	void print();
	void setVerbose(bool verbose);
private:
	unsigned int id;
	Pos pos;
	std::vector<PetriToken*> tokens;
	bool verbose = false;
};
