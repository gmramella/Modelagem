#pragma once

class Player
{
public:
	Player(unsigned int id, unsigned int x, unsigned int y, unsigned int w, unsigned int h);
	Player(Player* player);
	~Player();
	unsigned int getId();
	unsigned int getX();
	unsigned int getY();
	void goDown();
	void goLeft();
	void goRight();
	void goUp();
	void hMove(unsigned int hDir);
	void vMove(unsigned int vDir);
	bool getCanMove();
	void setCanMove(bool canMove);
	void decMoveCounter();
	unsigned int getMoveCounter();
	void setMoveCounter(unsigned int moveCounter);
private:
	unsigned int id;
	unsigned int x;
	unsigned int y;
	unsigned int w;
	unsigned int h;
	bool canMove = true;
	unsigned int moveCounter = 0;
	unsigned int numFlagsCaptured = 0;
};
