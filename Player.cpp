#include "Player.h"

Player::Player(unsigned int id, unsigned int x, unsigned int y, unsigned int w, unsigned int h) {
	this->id = id;
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
}

Player::Player(Player* player) {
	this->id = player->id;
	this->x = player->x;
	this->y = player->y;
	this->w = player->w;
	this->h = player->h;
}

Player::~Player() {
	id = 0;
	x = 0;
	y = 0;
	w = 0;
	h = 0;
}

unsigned int Player::getId()
{
	return id;
}

unsigned int Player::getX()
{
	return x;
}

unsigned int Player::getY()
{
	return y;
}

void Player::goDown()
{
	y ++;
	y = (y > h ? h : y);
}

void Player::goLeft()
{
	x --;
	x = (x > w ? w - 100 : x);
}

void Player::goRight()
{
	x ++;
	x = (x > w ? w : x);
}

void Player::goUp()
{
	y --;
	y = (y > h ? h - 100 : y);
}

void Player::hMove(unsigned int hDir)
{
	x += hDir;
}

void Player::vMove(unsigned int vDir)
{
	y += vDir;
}

bool Player::getCanMove()
{
	return canMove;
}

void Player::setCanMove(bool canMove)
{
	this->canMove = canMove;
}

void Player::decMoveCounter()
{
	moveCounter--;
}

unsigned int Player::getMoveCounter()
{
	return moveCounter;
}

void Player::setMoveCounter(unsigned int moveCounter)
{
	this->moveCounter = moveCounter;
}
