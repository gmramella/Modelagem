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

enum GTile { tEmpty, tWall, tPlayer, tNpc, tFlag, tFlooded };

struct GridTile {
	int x, y;
	GTile type;
	Sprite sprite;
};

struct Movement {
	int hDir;
	int vDir;
	Movement(int hDir, int vDir) {
		this->hDir = hDir;
		this->vDir = vDir;
	}
};

class GridGenerator
{
public:
	void generate(bool** terrain, unsigned int width, unsigned int height);
	unsigned int addPlayers(unsigned int nPlayers = 1);
	void addFlags(unsigned int playersQuadrant, unsigned int nFlags = 3);
	void addNpcs(unsigned int playersQuadrant, unsigned int nNpcs = 3);
	void flood(GTile targetType, GTile replacementType);
	void fill();
	void evaporate();
	void setSpritesheets();
	std::vector<Movement> update();
	void draw();
	GridTile** getGrid();
	std::vector<Player> getPlayers();
	std::vector<Npc> getNpcs();
	std::vector<Flag> getFlags();
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

	unsigned int manhattanDistance(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
	bool farFromPlayers(unsigned int x, unsigned int y, unsigned int minDist);
	bool nearFlags(unsigned int x, unsigned int y, unsigned int maxDist);
};
