#include "GridGenerator.h"

void GridGenerator::generate(bool** terrain, unsigned int width, unsigned int height)
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
			if (grid[i][j].type == tEmpty) {
				grid[i][j].dijkstraWeight = 1 + rand() % 9;
			}
			else if (grid[i][j].type == tPlayer) {
				grid[i][j].dijkstraWeight = 1;
			}
			else if (grid[i][j].type == tNpc) {
				grid[i][j].dijkstraWeight = INT_MAX;
			}
			else if (grid[i][j].type == tFlag) {
				grid[i][j].dijkstraWeight = 1;
			}
			else {
				grid[i][j].dijkstraWeight = INT_MAX;
			}
		}
	}
}

void GridGenerator::destroy(unsigned int width, unsigned int height)
{
	for (unsigned int i = 0; i < width; i++)
		delete[] grid[i];
	delete[] grid;

	quadrants.clear();
	players.clear();
	npcs.clear();
	flags.clear();
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

bool GridGenerator::sink(bool sinking, Movement dir)
{
	if (!sinking) {
		if (sinkHole) delete sinkHole;
		sinkHole = new Pos2(dir.hDir, dir.vDir);
		lastPos = *sinkHole;
	}
	else {
		lastPos += Pos2(dir.hDir, dir.vDir);
	}
	if ((0 <= lastPos.x && lastPos.x < width && 0 <= lastPos.y && lastPos.y < height) && (grid[lastPos.x][lastPos.y].type == tEmpty || grid[lastPos.x][lastPos.y].type == tWall)) {
		grid[lastPos.x][lastPos.y].type = tFlooded;
		grid[lastPos.x][lastPos.y].sprite.setSpriteSheet("flooded");
		return true;
	}
	else {
		lastPos -= Pos2(dir.hDir, dir.vDir);
		return false;
	}
}

void GridGenerator::drawPath(std::vector<Pos2> pathPos)
{
	for (std::vector<Pos2>::iterator it = pathPos.begin()+1; it != pathPos.end()-1; it++) {
		grid[it->x][it->y].type = tPath;
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
				grid[i][j].sprite.setSpriteSheet("empty" + (weightsVisible ? to_string(grid[i][j].dijkstraWeight) : ""));
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
			case tPath:
				grid[i][j].sprite.setSpriteSheet("path");
				break;
			}
		}
	}
}

Movements GridGenerator::update()
{
	Movements output;

	bool alive = true;
	for (std::vector<Npc>::iterator n = npcs.begin(); n != npcs.end(); n++) {
		if (n->getCanMove()) {
			Pos2 nPos(n->getX(), n->getY());
			unsigned int minDist = UINT_MAX;
			unsigned int x = 1;
			unsigned int y = 1;
			for (std::vector<Player>::iterator p = players.begin(); p != players.end(); p++) {
				Pos2 pPos(p->getX(), p->getY());
				unsigned int dists[3][3];
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 3; j++) {
						if ((nPos.x == 0 && i == 0) || (nPos.x == width - 1 && i == 2) || (nPos.y == 0 && j == 0) || (nPos.y == height - 1 && j == 2)) dists[i][j] = UINT_MAX;
						else dists[i][j] = manhattanDistance(nPos.x + (i - 1), nPos.y + (j - 1), pPos.x, pPos.y);
					}
				}
				unsigned int min = UINT_MAX;
				unsigned int minX = 1;
				unsigned int minY = 1;
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 3; j++) {
						if (min > dists[i][j]) {
							min = dists[i][j];
							minX = i;
							minY = j;
						}
					}
				}
				if (minDist > min) {
					minDist = min;
					x = minX;
					y = minY;
				}
			}

			int hDir = 0;
			int vDir = 0;

			if (y == 2)
			{
				vDir += 1;
			}
			if (x == 0)
			{
				hDir -= 1;
			}
			if (x == 2)
			{
				hDir += 1;
			}
			if (y == 0)
			{
				vDir -= 1;
			}

			if (grid[n->getX() + hDir][n->getY() + vDir].type == tEmpty || grid[n->getX() + hDir][n->getY() + vDir].type == tPath)
			{
				grid[n->getX()][n->getY()].type = tEmpty;
				unsigned int weight = grid[n->getX()][n->getY()].dijkstraWeight;
				grid[n->getX()][n->getY()].sprite.setSpriteSheet("empty" + (weightsVisible ? to_string(weight) : ""));
				n->hMove(hDir);
				n->vMove(vDir);
				grid[n->getX()][n->getY()].type = tNpc;
				grid[n->getX()][n->getY()].sprite.setSpriteSheet("npc");
				output.npcsMovements.push_back(Movement(hDir, vDir));
			}
			else if (grid[n->getX() + hDir][n->getY() + vDir].type == tPlayer)
			{
				grid[n->getX()][n->getY()].type = tEmpty;
				unsigned int weight = grid[n->getX()][n->getY()].dijkstraWeight;
				grid[n->getX()][n->getY()].sprite.setSpriteSheet("empty" + (weightsVisible ? to_string(weight) : ""));
				n->hMove(hDir);
				n->vMove(vDir);
				grid[n->getX()][n->getY()].type = tNpc;
				grid[n->getX()][n->getY()].sprite.setSpriteSheet("npc");
				output.npcsMovements.push_back(Movement(hDir, vDir));
				alive = false;
			}
			else
			{
				output.npcsMovements.push_back(Movement(0, 0));
			}

			double dist = sqrt((hDir * hDir) + (vDir * vDir));
			if (dist == 0) dist = 1;
			n->setMoveCounter((int)floor(gTempo.getLimiteFPS() * dist / 10));
			n->setCanMove(false);
		}
		else {
			n->decMoveCounter();
			if (n->getMoveCounter() == 0)
			{
				n->setCanMove(true);
			}
			output.playersMovements.push_back(Movement(0, 0));
		}
	}
	output.allAlive = alive;

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

			if (grid[players[i].getX() + hDir][players[i].getY() + vDir].type == tEmpty || grid[players[i].getX() + hDir][players[i].getY() + vDir].type == tPath)
			{
				grid[players[i].getX()][players[i].getY()].type = tEmpty;
				unsigned int weight = grid[players[i].getX()][players[i].getY()].dijkstraWeight;
				grid[players[i].getX()][players[i].getY()].sprite.setSpriteSheet("empty" + (weightsVisible ? to_string(weight) : ""));
				players[i].hMove(hDir);
				players[i].vMove(vDir);
				grid[players[i].getX()][players[i].getY()].type = tPlayer;
				grid[players[i].getX()][players[i].getY()].sprite.setSpriteSheet("player");
				output.playersMovements.push_back(Movement(hDir, vDir));
			}
			else if (grid[players[i].getX() + hDir][players[i].getY() + vDir].type == tFlag)
			{
				grid[players[i].getX()][players[i].getY()].type = tEmpty;
				unsigned int weight = grid[players[i].getX()][players[i].getY()].dijkstraWeight;
				grid[players[i].getX()][players[i].getY()].sprite.setSpriteSheet("empty" + (weightsVisible ? to_string(weight) : ""));
				players[i].hMove(hDir);
				players[i].vMove(vDir);
				grid[players[i].getX()][players[i].getY()].type = tPlayer;
				grid[players[i].getX()][players[i].getY()].sprite.setSpriteSheet("player");
				output.playersMovements.push_back(Movement(hDir, vDir));
				
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
				output.playersMovements.push_back(Movement(0, 0));
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
			output.playersMovements.push_back(Movement(0, 0));
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

void GridGenerator::toggleWeightsVisible()
{
	weightsVisible = !weightsVisible;
}

unsigned int GridGenerator::manhattanDistance(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
	unsigned int x = (x1 > x2) ? x1 - x2 : x2 - x1;
	unsigned int y = (y1 > y2) ? y1 - y2 : y2 - y1;
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
