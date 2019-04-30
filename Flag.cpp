#include "Flag.h"

Flag::Flag(unsigned int id, unsigned int x, unsigned int y) {
	this->id = id;
	this->x = x;
	this->y = y;
}

Flag::Flag(Flag* flag) {
	this->id = flag->id;
	this->x = flag->x;
	this->y = flag->y;
}

Flag::~Flag() {
	id = 0;
	x = 0;
	y = 0;
}

unsigned int Flag::getId()
{
	return id;
}

unsigned int Flag::getX()
{
	return x;
}

unsigned int Flag::getY()
{
	return y;
}
