#include "TerrainGenerator.h"

TerrainGenerator::TerrainGenerator(int width, int height)
{
	this->width = width;
	this->height = height;
}

TerrainGenerator::~TerrainGenerator()
{
	if (matrix != nullptr)
	{
		for (unsigned int i = 0; i < width; i++)
			delete[] matrix[i];
		delete[] matrix;
	}
	if (nextCycle != nullptr)
	{
		for (unsigned int i = 0; i < width; i++)
			delete[] nextCycle[i];
		delete[] nextCycle;
	}
}

bool** TerrainGenerator::generate(bool wallify, bool verbose)
{
	matrix = new bool*[width];
	for (unsigned int i = 0; i < width; i++)
		matrix[i] = new bool[height];

	nextCycle = new bool*[width];
	for (unsigned int i = 0; i < width; i++)
		nextCycle[i] = new bool[height];

	init();
	if (verbose)
	{
		system("@cls||clear");
		print();
	}
	for (unsigned int i = 0; i < 5; i++)
	{
		cycle();
		if (verbose)
		{
			system("@cls||clear");
			print();
		}
	}
	if (wallify) {
		walls();
		if (verbose)
		{
			system("@cls||clear");
			print();
		}
	}
	return matrix;
}

bool TerrainGenerator::atLeastOneAlive()
{
	for (unsigned int i = 0; i < width; i++)
	{
		for (unsigned int j = 0; j < height; j++)
		{
			if (matrix[i][j])
				return true;
		}
	}
	return false;
}

char TerrainGenerator::boolToChar(bool in)
{
	return (in ? YAY : NAY);
}

unsigned int TerrainGenerator::neighborsAlive(unsigned int i, unsigned int j)
{
	unsigned int w = 3;//02,03,05
	unsigned int h = 11;//07,11,13
	if (i == 0) w = 2; else if (i == width - 1) w = 5;
	if (j == 0) h = 7; else if (j == height - 1) h = 13;
	unsigned int count = 0;
	unsigned int l = i - 1;
	unsigned int t = j - 1;
	switch (w * h)
	{
	case 2 * 7://upper left
		l = width - 1;
		t = height - 1;
		break;
	case 2 * 11://upper row
		l = i - 1;
		t = height - 1;
		break;
	case 2 * 13://upper right
		l = i - 1;
		t = height - 1;
		break;
	case 3 * 7://left col
		l = width - 1;
		t = j - 1;
		break;
	case 3 * 11://middle col
		l = i - 1;
		t = j - 1;
		break;
	case 3 * 13://right col
		l = i - 1;
		t = j - 1;
		break;
	case 5 * 7://lower left
		l = width - 1;
		t = j - 1;
		break;
	case 5 * 11://lower row
		l = i - 1;
		t = j - 1;
		break;
	case 5 * 13://lower right
		l = i - 1;
		t = j - 1;
		break;
	}
	matrix[(l + 0) % width][(t + 0) % height] ? count++ : count;
	matrix[(l + 1) % width][(t + 0) % height] ? count++ : count;
	matrix[(l + 2) % width][(t + 0) % height] ? count++ : count;

	matrix[(l + 0) % width][(t + 1) % height] ? count++ : count;
	//matrix[(l + 1) % width][(t + 1) % height] ? count++ : count;
	matrix[(l + 2) % width][(t + 1) % height] ? count++ : count;

	matrix[(l + 0) % width][(t + 2) % height] ? count++ : count;
	matrix[(l + 1) % width][(t + 2) % height] ? count++ : count;
	matrix[(l + 2) % width][(t + 2) % height] ? count++ : count;
	return count;
}

bool TerrainGenerator::ruleCave(bool currState, unsigned int count)
{
	if (currState == ALIVE && count < 3) return DEAD;//empty
	if (currState == DEAD && count > 4) return ALIVE;//fill
	return currState;//remain
}

bool TerrainGenerator::ruleConway(bool currState, unsigned int count)
{
	if (currState == DEAD && count == 3) return ALIVE;//birth
	if (currState == ALIVE && (count == 2 || count == 3)) return ALIVE;//survival
	return DEAD;//death
}

bool TerrainGenerator::rule(bool currState, unsigned int count)
{
	return ruleCave(currState, count);
}

void TerrainGenerator::init()
{
	srand((unsigned int)time(NULL));
	for (unsigned int i = 0; i < width; i++)
	{
		for (unsigned int j = 0; j < height; j++)
		{
			matrix[i][j] = (rand() % 100) < 35 ? 1 : 0;
		}
	}
}

void TerrainGenerator::print() {
	for (unsigned int i = 0; i < width; i++)
	{
		for (unsigned int j = 0; j < height; j++)
		{
			std::cout << boolToChar(matrix[i][j]);
		}
		std::cout << std::endl;
	}
}

void TerrainGenerator::cycle() {
	for (unsigned int i = 0; i < width; i++)
	{
		for (unsigned int j = 0; j < height; j++)
		{
			unsigned int count = neighborsAlive(i, j);
			nextCycle[i][j] = rule(matrix[i][j], count);
		}
	}
	for (unsigned int i = 0; i < width; i++)
	{
		for (unsigned int j = 0; j < height; j++)
		{
			matrix[i][j] = nextCycle[i][j];
		}
	}
}

void TerrainGenerator::walls() {
	for (unsigned int i = 0; i < width; i++)
	{
		matrix[i][0] = ALIVE;//left
		matrix[i][height - 1] = ALIVE;//right
	}
	for (unsigned int j = 0; j < height; j++)
	{
		matrix[0][j] = ALIVE;//top
		matrix[width - 1][j] = ALIVE;//bottom
	}
}
