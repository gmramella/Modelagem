#pragma once
#include <cstdlib>
#include <ctime>
#include <iostream>

#define NAY (char)32
#ifdef _WIN32
#define YAY (char)219
#endif
#ifdef unix
#define YAY (char)35
#endif
#define DEAD false
#define ALIVE true

class TerrainGenerator
{
public:
	TerrainGenerator(int width, int height);
	~TerrainGenerator();
	bool** generate(bool wallify = true, bool verbose = false);
private:
	unsigned int width;
	unsigned int height;
	bool wallify;
	bool verbose;
	bool** matrix = nullptr;
	bool** nextCycle = nullptr;

	bool atLeastOneAlive();
	char boolToChar(bool in);
	unsigned int neighborsAlive(unsigned int i, unsigned int j);
	bool ruleCave(bool currState, unsigned int count);
	bool ruleConway(bool currState, unsigned int count);
	bool rule(bool currState, unsigned int count);

	void init();
	void print();
	void cycle();
	void walls();
};
