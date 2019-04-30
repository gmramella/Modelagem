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
private:
	unsigned int id;
	unsigned int x;
	unsigned int y;
	unsigned int w;
	unsigned int h;
};
