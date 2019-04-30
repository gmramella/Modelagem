#pragma once

class Flag
{
public:
	Flag(unsigned int id, unsigned int x, unsigned int y);
	Flag(Flag* flag);
	~Flag();
	unsigned int getId();
	unsigned int getX();
	unsigned int getY();
private:
	unsigned int id;
	unsigned int x;
	unsigned int y;
};
