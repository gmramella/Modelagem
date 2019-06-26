#pragma once
#include "Place.h"
#include "Transition.h"
#include "GridGenerator.h"

struct PairTokenOrigin {
	PetriToken* token;
	Transition* origin;
};

class PetriNet
{
public:
	PetriNet(unsigned int width = 0, unsigned int height = 0, bool verbose = false);
	~PetriNet();
	bool addPlace(Place* place);
	Place* delPlace(Place* place);
	std::vector<Place*> getPlaces();
	bool addTransition(Transition* transition);
	Transition* delTransition(Transition* transition);
	std::vector<Transition*> getTransitions();
	void runCycle(Movements movements, std::vector<Player> players, std::vector<Npc> npcs);
	void showStatus();
	bool saveNet();
	bool loadNet();
	void setTokenEnteringPlaceCallback(void(*func)(PetriToken, Place));
	void setTokenLeavingPlaceCallback(void(*func)(PetriToken, Place));
	void setTransitionEnableCallback(void(*func)(Transition));
	void setTransitionFireCallback(void(*func)(Transition));
	void setVerbose(bool verbose);
	std::vector<Connection*> getPlaceConnections(Place place);

	std::vector<Place*> playersPlacesPtr;
	std::vector<Place*> flagsPlacesPtr;
	std::vector<Place*> npcsPlacesPtr;
	std::vector<Transition*> playersInTransitionsPtr;
	std::vector<Transition*> playersOutTransitionsPtr;
	std::vector<Transition*> flagsInTransitionsPtr;
	std::vector<Transition*> flagsOutTransitionsPtr;
	std::vector<Transition*> npcsInTransitionsPtr;
	std::vector<Transition*> npcsOutTransitionsPtr;

	void generate(GridGenerator grid, unsigned int width, unsigned int height, void(*tepc)(PetriToken, Place) = nullptr, void(*tlpc)(PetriToken, Place) = nullptr, void(*tec)(Transition) = nullptr, void(*tfc)(Transition) = nullptr);
	void destroy();
	std::vector<Place*> places2;
	std::vector<Transition*> transitions2;
private:
	unsigned int width;
	unsigned int height;
	std::vector<Place*> places;
	std::vector<Transition*> transitions;
	void(*tokenEnteringPlaceCallback)(PetriToken, Place) = nullptr;
	void(*tokenLeavingPlaceCallback)(PetriToken, Place) = nullptr;
	void(*transitionEnableCallback)(Transition) = nullptr;
	void(*transitionFireCallback)(Transition) = nullptr;
	bool verbose = false;

	int godelToIndex(int g) {
		if (g == (int)(pow(2, 1) * pow(3, 1))) return 0;
		if (g == (int)(pow(2, 1) * pow(3, 2))) return 1;
		if (g == (int)(pow(2, 1) * pow(3, 3))) return 2;
		if (g == (int)(pow(2, 2) * pow(3, 1))) return 3;
		if (g == (int)(pow(2, 2) * pow(3, 3))) return 4;
		if (g == (int)(pow(2, 3) * pow(3, 1))) return 5;
		if (g == (int)(pow(2, 3) * pow(3, 2))) return 6;
		if (g == (int)(pow(2, 3) * pow(3, 3))) return 7;
		else return -1;
	}
};
