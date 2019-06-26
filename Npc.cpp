#include "Npc.h"

Npc::Npc(unsigned int id, unsigned int x, unsigned int y, unsigned int w, unsigned int h) {
	this->id = id;
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
}

Npc::Npc(Npc* npc) {
	this->id = npc->id;
	this->x = npc->x;
	this->y = npc->y;
	this->w = npc->w;
	this->h = npc->h;
}

Npc::~Npc() {
	id = 0;
	x = 0;
	y = 0;
	w = 0;
	h = 0;
}

unsigned int Npc::getId()
{
	return id;
}

unsigned int Npc::getX()
{
	return x;
}

unsigned int Npc::getY()
{
	return y;
}

void Npc::hMove(unsigned int hDir)
{
	x += hDir;
}

void Npc::vMove(unsigned int vDir)
{
	y += vDir;
}

bool Npc::getCanMove()
{
	return canMove;
}

void Npc::setCanMove(bool canMove)
{
	this->canMove = canMove;
}

void Npc::decMoveCounter()
{
	moveCounter--;
}

unsigned int Npc::getMoveCounter()
{
	return moveCounter;
}

void Npc::setMoveCounter(unsigned int moveCounter)
{
	this->moveCounter = moveCounter;
}
