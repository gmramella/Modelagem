#pragma once
#include <cstdlib>
#include <ctime>

class Randomizer
{
public:
	double uniform(double inf, double sup);
	double exponential(double avg);
	double normal(double avg, double sd);
private:
	double generateRnd();
};

