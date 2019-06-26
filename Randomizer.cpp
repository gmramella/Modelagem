#include "Randomizer.h"

double Randomizer::uniform(double inf, double sup)
{
	return inf + (sup - inf) * generateRnd();
}

double Randomizer::exponential(double avg)
{
	double x = generateRnd();
	while (x == 1.0)
		x = generateRnd();
	return -avg * log(1 - x);
}

double Randomizer::normal(double avg, double sd)
{
	double U1 = generateRnd();
	double U2 = generateRnd();
	double V1 = 2 * U1 - 1;
	double V2 = 2 * U2 - 1;
	double W = V1 * V1 + V2 * V2;
	while (W > 1) {
		U1 = generateRnd();
		U2 = generateRnd();
		V1 = 2 * U1 - 1;
		V2 = 2 * U2 - 1;
		W = V1 * V1 + V2 * V2;
	}
	double y = sqrt(-2 * log(W) / W);
	return avg + sd * (V1 * y);
}

double Randomizer::generateRnd()
{
	long int a = 1103515245;
	long int c = 12345;
	long int m = 2147483648;//2^31

	srand((unsigned int)time(NULL));
	long int x = rand() % 1103515245;
	return (((a * x + c) % m) & 2147483647) / (double)(m - 1);
}
