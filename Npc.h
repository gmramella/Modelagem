#pragma once

class Npc
{
public:
	Npc(unsigned int id, unsigned int x, unsigned int y, unsigned int w, unsigned int h);
	Npc(Npc* npc);
	~Npc();
	unsigned int getId();
	unsigned int getX();
	unsigned int getY();
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
};
