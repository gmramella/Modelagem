#include "GridGenerator.h"

void GridGenerator::generate(bool ** terrain, unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;
	this->grid = new GridTile*[width];
	for (unsigned int i = 0; i < width; i++)
		grid[i] = new GridTile[height];

	unsigned int spriteWidth = gRecursos.getSpriteSheet("empty")->getTextura()->getLargura();
	unsigned int spriteHeight = gRecursos.getSpriteSheet("empty")->getTextura()->getAltura();

	for (unsigned int i = 0; i < width; i++)
	{
		for (unsigned int j = 0; j < height; j++)
		{
			grid[i][j].type = (terrain[i][j] == 0) ? tEmpty : tWall;
			grid[i][j].x = spriteWidth * i + spriteWidth / 2;
			grid[i][j].y = spriteHeight * j + spriteHeight / 2;
		}
	}
}

unsigned int GridGenerator::addPlayers(unsigned int nPlayers)
{
	srand((unsigned int)time(NULL));
	unsigned int quadrant = rand() % 4;
	unsigned int minX = (width / 2) * (quadrant % 2);
	unsigned int minY = (height / 2) * (quadrant / 2);
	unsigned int maxX = (width / 2) * (1 + quadrant % 2);
	unsigned int maxY = (height / 2) * (1 + quadrant / 2);
	while (nPlayers > 0)
	{
		unsigned int x = rand() % width;
		unsigned int y = rand() % height;
		if (grid[x][y].type == tEmpty && (minX <= x && x <= maxX && minY <= y && y <= maxY))
		{
			grid[x][y].type = tPlayer;
			players.push_back(new Player((unsigned int)players.size(), x, y, width, height));
			nPlayers--;
		}
	}
	return quadrant;
}

void GridGenerator::addFlags(unsigned int playersQuadrant, unsigned int nFlags)
{
	srand((unsigned int)time(NULL));
	while (nFlags > 0)
	{
		unsigned int quadrant = rand() % 4;
		while (quadrant == playersQuadrant)
			quadrant = rand() % 4;
		unsigned int minX = (width / 2) * (quadrant % 2);
		unsigned int minY = (height / 2) * (quadrant / 2);
		unsigned int maxX = (width / 2) * (1 + quadrant % 2);
		unsigned int maxY = (height / 2) * (1 + quadrant / 2);

		unsigned int x = rand() % width;
		unsigned int y = rand() % height;
		while (!(minX <= x && x <= maxX && minY <= y && y <= maxY))
		{
			x = rand() % width;
			y = rand() % height;
		}
		if (grid[x][y].type == tFlooded && (minX <= x && x <= maxX && minY <= y && y <= maxY))
		{
			grid[x][y].type = tFlag;
			flags.push_back(new Flag((unsigned int)flags.size(), x, y));
			nFlags--;
		}
	}
}

void GridGenerator::addNpcs(unsigned int playersQuadrant, unsigned int nNpcs)
{
	unsigned int nFlags = flags.size();
	if (nFlags == 0)
	{
		while (nNpcs > 0)
		{
			unsigned int quadrant = rand() % 4;
			while (quadrant == playersQuadrant)
				quadrant = rand() % 4;
			unsigned int minX = (width / 2) * (quadrant % 2);
			unsigned int minY = (height / 2) * (quadrant / 2);
			unsigned int maxX = (width / 2) * (1 + quadrant % 2);
			unsigned int maxY = (height / 2) * (1 + quadrant / 2);

			unsigned int x = rand() % width;
			unsigned int y = rand() % height;
			while (!(minX <= x && x <= maxX && minY <= y && y <= maxY))
			{
				x = rand() % width;
				y = rand() % height;
			}
			if (grid[x][y].type == tFlooded)
			{
				grid[x][y].type = tNpc;
				npcs.push_back(new Npc((unsigned int)npcs.size(), x, y, width, height));
				nNpcs--;
			}
			else
			{
				if (x > 0 && grid[x - 1][y].type == tFlooded)
				{
					grid[x - 1][y].type = tNpc;
					npcs.push_back(new Npc((unsigned int)npcs.size(), x, y, width, height));
					nNpcs--;
				}
				else if (x < width - 1 && grid[x + 1][y].type == tFlooded)
				{
					grid[x + 1][y].type = tNpc;
					npcs.push_back(new Npc((unsigned int)npcs.size(), x, y, width, height));
					nNpcs--;
				}
				else if (y > 0 && grid[x][y - 1].type == tFlooded)
				{
					grid[x][y - 1].type = tNpc;
					npcs.push_back(new Npc((unsigned int)npcs.size(), x, y, width, height));
					nNpcs--;
				}
				else if (y < height - 1 && grid[x][y + 1].type == tFlooded)
				{
					grid[x][y + 1].type = tNpc;
					npcs.push_back(new Npc((unsigned int)npcs.size(), x, y, width, height));
					nNpcs--;
				}
			}
		}
	}
	else
	{
		if (nNpcs >= nFlags)
		{
			while (nNpcs > 0)
			{
				unsigned int index = (nFlags - nNpcs) % nFlags;
				unsigned int x = flags[index].getX() + (unsigned int)pow(-1, rand() % 2) * rand() % width / 8;
				unsigned int y = flags[index].getY() + (unsigned int)pow(-1, rand() % 2) * rand() % height / 8;
				while (x == flags[index].getX() || y == flags[index].getY() || x < 0 || x >= width || y < 0 || y >= height)
				{
					x = flags[index].getX() + (unsigned int)pow(-1, rand() % 2) * rand() % width / 8;
					y = flags[index].getY() + (unsigned int)pow(-1, rand() % 2) * rand() % height / 8;
				}
				if (grid[x][y].type == tFlooded)
				{
					grid[x][y].type = tNpc;
					npcs.push_back(new Npc((unsigned int)npcs.size(), x, y, width, height));
					nNpcs--;
				}
				else
				{
					if (x > 0 && grid[x - 1][y].type == tFlooded)
					{
						grid[x - 1][y].type = tNpc;
						npcs.push_back(new Npc((unsigned int)npcs.size(), x, y, width, height));
						nNpcs--;
					}
					else if (x < width - 1 && grid[x + 1][y].type == tFlooded)
					{
						grid[x + 1][y].type = tNpc;
						npcs.push_back(new Npc((unsigned int)npcs.size(), x, y, width, height));
						nNpcs--;
					}
					else if (y > 0 && grid[x][y - 1].type == tFlooded)
					{
						grid[x][y - 1].type = tNpc;
						npcs.push_back(new Npc((unsigned int)npcs.size(), x, y, width, height));
						nNpcs--;
					}
					else if (y < height - 1 && grid[x][y + 1].type == tFlooded)
					{
						grid[x][y + 1].type = tNpc;
						npcs.push_back(new Npc((unsigned int)npcs.size(), x, y, width, height));
						nNpcs--;
					}
				}
			}
		}
		else
		{
			while (nNpcs > 0)
			{
				unsigned int index = (nFlags - nNpcs) % nFlags;
				unsigned int x = flags[index].getX() + (unsigned int)pow(-1, rand() % 2) * rand() % width / 8;
				unsigned int y = flags[index].getY() + (unsigned int)pow(-1, rand() % 2) * rand() % height / 8;
				while (x == flags[index].getX() || y == flags[index].getY() || x < 0 || x >= width || y < 0 || y >= height)
				{
					x = flags[index].getX() + (unsigned int)pow(-1, rand() % 2) * rand() % width / 8;
					y = flags[index].getY() + (unsigned int)pow(-1, rand() % 2) * rand() % height / 8;
				}
				if (grid[x][y].type == tFlooded)
				{
					grid[x][y].type = tNpc;
					npcs.push_back(new Npc((unsigned int)npcs.size(), x, y, width, height));
					nNpcs--;
				}
				else
				{
					if (x > 0 && grid[x - 1][y].type == tFlooded)
					{
						grid[x - 1][y].type = tNpc;
						npcs.push_back(new Npc((unsigned int)npcs.size(), x, y, width, height));
						nNpcs--;
					}
					else if (x < width - 1 && grid[x + 1][y].type == tFlooded)
					{
						grid[x + 1][y].type = tNpc;
						npcs.push_back(new Npc((unsigned int)npcs.size(), x, y, width, height));
						nNpcs--;
					}
					else if (y > 0 && grid[x][y - 1].type == tFlooded)
					{
						grid[x][y - 1].type = tNpc;
						npcs.push_back(new Npc((unsigned int)npcs.size(), x, y, width, height));
						nNpcs--;
					}
					else if (y < height - 1 && grid[x][y + 1].type == tFlooded)
					{
						grid[x][y + 1].type = tNpc;
						npcs.push_back(new Npc((unsigned int)npcs.size(), x, y, width, height));
						nNpcs--;
					}
				}
			}
		}
	}
}

void GridGenerator::flood(GTile targetType, GTile replacementType)
{
	for (size_t i = 0; i < players.size(); i++)
	{
		GridTile initial = grid[players[i].getX()][players[i].getY()];
		initial.type = replacementType;
		std::vector<GridTile> stack;
		stack.push_back(initial);
		unsigned int spriteWidth = gRecursos.getSpriteSheet("empty")->getTextura()->getLargura();
		unsigned int spriteHeight = gRecursos.getSpriteSheet("empty")->getTextura()->getAltura();
		while (stack.size() > 0)
		{
			GridTile first = stack[0];
			stack.erase(stack.begin());
			unsigned int x = first.x / spriteWidth;
			unsigned int y = first.y / spriteHeight;
			if (x > 0)
			{
				GridTile* west = &grid[x - 1][y];
				if (west->type == targetType)
				{
					west->type = replacementType;
					stack.push_back(*west);
				}
			}
			if (x < width - 1)
			{
				GridTile* east = &grid[x + 1][y];
				if (east->type == targetType)
				{
					east->type = replacementType;
					stack.push_back(*east);
				}
			}
			if (y > 0)
			{
				GridTile* north = &grid[x][y - 1];
				if (north->type == targetType)
				{
					north->type = replacementType;
					stack.push_back(*north);
				}
			}
			if (y < height - 1)
			{
				GridTile* south = &grid[x][y + 1];
				if (south->type == targetType)
				{
					south->type = replacementType;
					stack.push_back(*south);
				}
			}
		}
	}
}

void GridGenerator::fill()
{
	for (unsigned int i = 0; i < width; i++)
	{
		for (unsigned int j = 0; j < height; j++)
		{
			if (grid[i][j].type == tEmpty)
				grid[i][j].type = tWall;
		}
	}
}

void GridGenerator::evaporate()
{
	for (unsigned int i = 0; i < width; i++)
	{
		for (unsigned int j = 0; j < height; j++)
		{
			if (grid[i][j].type == tFlooded)
				grid[i][j].type = tEmpty;
		}
	}
}

void GridGenerator::setSpritesheets()
{
	for (unsigned int i = 0; i < width; i++)
	{
		for (unsigned int j = 0; j < height; j++)
		{
			switch (grid[i][j].type)
			{
			case tEmpty:
				grid[i][j].sprite.setSpriteSheet("empty");
				break;
			case tWall:
				grid[i][j].sprite.setSpriteSheet("wall");
				break;
			case tPlayer:
				grid[i][j].sprite.setSpriteSheet("player");
				break;
			case tNpc:
				grid[i][j].sprite.setSpriteSheet("npc");
				break;
			case tFlag:
				grid[i][j].sprite.setSpriteSheet("flag");
				break;
			case tFlooded:
				grid[i][j].sprite.setSpriteSheet("flooded");
				break;
			}
		}
	}
}

std::vector<Movement> GridGenerator::update()
{
	std::vector<Movement> output;

	for (size_t i = 0; i < players.size(); i++)
	{
		if (players[i].getCanMove())
		{
			int hDir = 0;
			int vDir = 0;

			if (gTeclado.pressionou[TECLA_BAIXO] || gTeclado.segurando[TECLA_BAIXO])
			{
				vDir += 1;
			}
			if (gTeclado.pressionou[TECLA_ESQ] || gTeclado.segurando[TECLA_ESQ])
			{
				hDir -= 1;
			}
			if (gTeclado.pressionou[TECLA_DIR] || gTeclado.segurando[TECLA_DIR])
			{
				hDir += 1;
			}
			if (gTeclado.pressionou[TECLA_CIMA] || gTeclado.segurando[TECLA_CIMA])
			{
				vDir -= 1;
			}

			if (grid[players[i].getX() + hDir][players[i].getY() + vDir].type == tEmpty)
			{
				grid[players[i].getX()][players[i].getY()].type = tEmpty;
				grid[players[i].getX()][players[i].getY()].sprite.setSpriteSheet("empty");
				players[i].hMove(hDir);
				players[i].vMove(vDir);
				grid[players[i].getX()][players[i].getY()].type = tPlayer;
				grid[players[i].getX()][players[i].getY()].sprite.setSpriteSheet("player");
				output.push_back(Movement(hDir, vDir));
			}
			else if (grid[players[i].getX() + hDir][players[i].getY() + vDir].type == tFlag)
			{
				grid[players[i].getX()][players[i].getY()].type = tEmpty;
				grid[players[i].getX()][players[i].getY()].sprite.setSpriteSheet("empty");
				players[i].hMove(hDir);
				players[i].vMove(vDir);
				grid[players[i].getX()][players[i].getY()].type = tPlayer;
				grid[players[i].getX()][players[i].getY()].sprite.setSpriteSheet("player");
				output.push_back(Movement(hDir, vDir));
				
				for (std::vector<Flag>::iterator flag = flags.begin(); flag != flags.end(); flag++) {
					if (flag->getX() == players[i].getX() && flag->getY() == players[i].getY())
					{
						flags.erase(flag);
						break;
					}
				}
			}
			else
			{
				output.push_back(Movement(0, 0));
			}

			double dist = sqrt((hDir * hDir) + (vDir * vDir));
			if (dist == 0) dist = 1;
			players[i].setMoveCounter((int)floor(gTempo.getLimiteFPS() * dist / 10));
			players[i].setCanMove(false);
		}
		else
		{
			players[i].decMoveCounter();
			if (players[i].getMoveCounter() == 0)
			{
				players[i].setCanMove(true);
			}
			output.push_back(Movement(0, 0));
		}
	}

	return output;
}

void GridGenerator::draw()
{
	for (unsigned int i = 0; i < width; i++)
	{
		for (unsigned int j = 0; j < height; j++)
		{
			GridTile tile = grid[i][j];
			tile.sprite.desenhar(tile.x, tile.y);
		}
	}
}

GridTile ** GridGenerator::getGrid()
{
	return grid;
}

std::vector<Player> GridGenerator::getPlayers()
{
	return players;
}

std::vector<Npc> GridGenerator::getNpcs()
{
	return npcs;
}

std::vector<Flag> GridGenerator::getFlags()
{
	return flags;
}

unsigned int GridGenerator::manhattanDistance(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
	unsigned int x = (x1 > x2) ? x1 : x2;
	unsigned int y = (y1 > y2) ? y1 : y2;
	return x + y;
}

bool GridGenerator::farFromPlayers(unsigned int x, unsigned int y, unsigned int minDist)
{
	for (size_t i = 0; i < players.size(); i++)
	{
		if (manhattanDistance(x, y, players[i].getX(), players[i].getY()) < minDist)
			return false;
	}
	return true;
}

bool GridGenerator::nearFlags(unsigned int x, unsigned int y, unsigned int maxDist)
{
	for (size_t i = 0; i < flags.size(); i++)
	{
		if (manhattanDistance(x, y, flags[i].getX(), flags[i].getY()) > maxDist)
			return false;
	}
	return true;
}
