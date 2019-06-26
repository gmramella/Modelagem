#pragma once
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <cmath>
#include "libUnicornio.h"
#include "Player.h"
#include "Npc.h"
#include "Flag.h"

#define ABOVE(x, y) (grid[x][y - 1])
#define BELOW(x, y) (grid[x][y + 1])
#define LEFT(x, y) (grid[x - 1][y])
#define RIGHT(x, y) (grid[x + 1][y])

enum GTile { tEmpty, tWall, tPlayer, tNpc, tFlag, tFlooded, tPath };

struct GridTile {
	int x, y;
	GTile type;
	Sprite sprite;
	int dijkstraWeight;
};

struct Movement {
	int hDir;
	int vDir;
	Movement(int hDir, int vDir) {
		this->hDir = hDir;
		this->vDir = vDir;
	}
};

struct Movements {
	std::vector<Movement> playersMovements;
	bool allAlive;
	std::vector<Movement> npcsMovements;
};

struct Pos2 {
	int x;
	int y;
	Pos2(int x, int y) {
		this->x = x;
		this->y = y;
	}
	Pos2& operator = (const Pos2& other)
	{
		x = other.x;
		y = other.y;
		return *this;
	}
	Pos2& operator += (const Pos2& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}
	Pos2& operator -= (const Pos2& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}
	Pos2& operator - (const Pos2& other)
	{
		return Pos2(x - other.x, y - other.y);
	}
};

class GridGenerator
{
public:
	void generate(bool** terrain, unsigned int width, unsigned int height);
	void destroy(unsigned int width, unsigned int height);
	unsigned int addPlayers(unsigned int nPlayers = 1);
	void addFlags(unsigned int playersQuadrant, unsigned int nFlags = 3);
	void addNpcs(unsigned int playersQuadrant, unsigned int nNpcs = 3);
	void flood(GTile targetType, GTile replacementType);
	void fill();
	void evaporate();
	bool sink(bool sinking, Movement dir);
	void drawPath(std::vector<Pos2> pathPos);
	void setSpritesheets();
	Movements update();
	void draw();
	GridTile** getGrid();
	std::vector<Player> getPlayers();
	std::vector<Npc> getNpcs();
	std::vector<Flag> getFlags();
	void toggleWeightsVisible();
	unsigned int width, height;
private:
	Sprite emptySprite;
	Sprite wallSprite;
	Sprite playerSprite;
	Sprite npcSprite;
	Sprite flagSprite;
	GridTile** grid;
	std::vector<unsigned int> quadrants;
	std::vector<Player> players;
	std::vector<Npc> npcs;
	std::vector<Flag> flags;
	Pos2* sinkHole = nullptr;
	Pos2 lastPos = Pos2(0, 0);
	bool weightsVisible = true;

	unsigned int manhattanDistance(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
	bool farFromPlayers(unsigned int x, unsigned int y, unsigned int minDist);
	bool nearFlags(unsigned int x, unsigned int y, unsigned int maxDist);
};
