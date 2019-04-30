#include "PetriNet.h"

PetriNet::PetriNet(unsigned int width, unsigned int height, bool verbose) {
	this->width = width;
	this->height = height;
	this->verbose = verbose;
}

PetriNet::~PetriNet() {
	width = 0;
	height = 0;
	places.clear();
	transitions.clear();
}

bool PetriNet::addPlace(Place* place) {
	return Utils::add(places, place);
}

Place* PetriNet::delPlace(Place* place) {
	return Utils::del(places, place);
}

std::vector<Place*> PetriNet::getPlaces() {
	return places;
}

bool PetriNet::addTransition(Transition* transition) {
	return Utils::add(transitions, transition);
}

Transition* PetriNet::delTransition(Transition* transition) {
	return Utils::del(transitions, transition);
}

std::vector<Transition*> PetriNet::getTransitions() {
	return transitions;
}

void PetriNet::runCycle(std::vector<Movement> playersMovements, std::vector<Player> players) {
	std::vector<Transition*> transitionsEnabled;
	std::vector<PetriToken*> copies;

	for (unsigned int i = 0; i < playersPlacesPtr.size(); i++) {
		if (playersMovements[i].hDir != 0 || playersMovements[i].vDir != 0) {
			Pos pos = playersPlacesPtr[i]->getPos();
			int index = width * pos.y + pos.x;
			std::cout << "(" << pos.x << ", " << pos.y << ")" << std::endl;
			std::cout << "(" << playersMovements[i].hDir << ", " << playersMovements[i].vDir << ")" << std::endl;
			double p2 = pow(2, (playersMovements[i].hDir + 2));
			double p3 = pow(3, (playersMovements[i].vDir + 2));
			int j = godelToIndex((int)(p2 * p3));
			Transition* tIn = playersInTransitionsPtr[8 * index + j];
			tIn->setIsEnabled(true);
			transitionsEnabled.push_back(tIn);
			if (transitionEnableCallback != nullptr) this->transitionEnableCallback(tIn);

			std::vector<Connection*> inputConnections = tIn->getInputConnections();
			for (std::vector<Connection*>::iterator connection = inputConnections.begin(); connection != inputConnections.end(); connection++) {
				if (!(*connection)->getIsInhibitor()) {
					unsigned int weight = (*connection)->getWeight();
					Place* place = (*connection)->getPlace();
					for (unsigned int i = 0; i < weight; i++) {
						/*PetriToken* first = place->getTokens()[0];
						PetriToken* copy = place->delToken(first);
						copies.push_back(copy);
						if (tokenLeavingPlaceCallback != nullptr) this->tokenLeavingPlaceCallback(*copy, *place);*/
					}
				}
			}
			
			Transition* tOut = playersOutTransitionsPtr[8 * index + j];
			std::vector<Connection*> outputConnections = tOut->getOutputConnections();
			for (std::vector<Connection*>::iterator connection = outputConnections.begin(); connection != outputConnections.end(); connection++) {
				unsigned int weight = (*connection)->getWeight();
				Place* place = (*connection)->getPlace();
				while (weight > 0) {
					/*PetriToken* temp = copies.at(0);
					copies.erase(copies.begin());
					place->addToken(temp);
					if (tokenEnteringPlaceCallback != nullptr) this->tokenEnteringPlaceCallback(*temp, *place);*/
					weight--;
				}
			}
		}
	}
	
	for (unsigned int i = 0; i < transitionsEnabled.size(); i++) {
		Transition* t = transitionsEnabled[i];
		if (transitionFireCallback != nullptr) this->transitionFireCallback(*t);
		t->setIsEnabled(false);
	}
	
	for (unsigned int i = 0; i < playersPlacesPtr.size(); i++) {
		if (playersMovements[i].hDir != 0 && playersMovements[i].vDir != 0) {
			//tirar place
			//tirar transitions
			//colocar place
			//colocar transitions

			playersPlacesPtr.clear();
			playersInTransitionsPtr.clear();
			playersOutTransitionsPtr.clear();
			flagsInTransitionsPtr.clear();
			flagsOutTransitionsPtr.clear();
			npcsInTransitionsPtr.clear();
			npcsOutTransitionsPtr.clear();

			for (unsigned int i = 0; i < players.size(); i++) {
				unsigned int x = players[i].getX();
				unsigned int y = players[i].getY();
				unsigned int index = width * y + x;
				std::cout << x << ", " << y << std::endl;
				std::cout << index << std::endl;
				playersPlacesPtr.push_back(places2.at(index));
			}
		}
	}
}

void PetriNet::showStatus() {
	std::cout << "Petri net has width of " << width << " and height of " << height << std::endl;
	unsigned int sumTokens = 0;
	unsigned int sumInputConnections = 0;
	unsigned int sumOutputConnections = 0;
	for (std::vector<Place*>::iterator it = places.begin(); it != places.end(); it++)
		sumTokens += (*it)->getTokens().size();
	for (std::vector<Transition*>::iterator it = transitions.begin(); it != transitions.end(); it++) {
		sumInputConnections += (*it)->getInputConnections().size();
		sumOutputConnections += (*it)->getOutputConnections().size();
	}
	std::cout << "Petri net has " << sumTokens << " token" << ((sumTokens != 1) ? "s" : "") << std::endl;
	std::cout << "Petri net has " << sumInputConnections << " input connection" << ((sumInputConnections != 1) ? "s" : "") << std::endl;
	std::cout << "Petri net has " << sumOutputConnections << " output connection" << ((sumOutputConnections != 1) ? "s" : "");
		
	if (verbose) {
		std::cout << ": " << std::endl;
		for (std::vector<Place*>::iterator it = places.begin(); it != places.end(); it++)
			(*it)->print();
		std::cout << "Petri net has " << transitions.size() << " transition" << ((transitions.size() != 1) ? "s: " : ": ") << std::endl;
		for (std::vector<Transition*>::iterator it = transitions.begin(); it != transitions.end(); it++)
			(*it)->print();
	}
	else {
		std::cout << std::endl;
	}
}

bool PetriNet::saveNet() {
	return false;
}

bool PetriNet::loadNet() {
	return false;
}

void PetriNet::setTokenEnteringPlaceCallback(void(*func)(PetriToken, Place)) {
	tokenEnteringPlaceCallback = func;
}

void PetriNet::setTokenLeavingPlaceCallback(void(*func)(PetriToken, Place)) {
	tokenLeavingPlaceCallback = func;
}

void PetriNet::setTransitionEnableCallback(void(*func)(Transition)) {
	transitionEnableCallback = func;
}

void PetriNet::setTransitionFireCallback(void(*func)(Transition)) {
	transitionFireCallback = func;
}

void PetriNet::setVerbose(bool verbose)
{
	this->verbose = verbose;
}

std::vector<Connection*> PetriNet::getPlaceConnections(Place place)
{
	std::vector<Connection*> out;
	for (std::vector<Transition*>::iterator transition = transitions.begin(); transition != transitions.end(); transition++) {
		std::vector<Connection*> inputConnections = (*transition)->getInputConnections();
		for (std::vector<Connection*>::iterator connection = inputConnections.begin(); connection != inputConnections.end(); connection++) {
			if ((*connection)->getPlace()->getId() == place.getId()) {
				out.push_back(*connection);
			}
		}
	}
	return out;
}

void PetriNet::generate(GridGenerator grid, unsigned int width, unsigned int height)
{
	std::vector<Player> players = grid.getPlayers();
	std::vector<Flag> flags = grid.getFlags();
	std::vector<Npc> npcs = grid.getNpcs();

	unsigned int petriTokenIdCounter = 0;
	unsigned int transitionIdCounter = 0;
	unsigned int connectionIdCounter = 0;

	for (unsigned int i = 0; i < width; i++) {
		for (unsigned int j = 0; j < height; j++) {
			places2.push_back(new Place(width * j + i, Pos(i, j), false));
		}
	}

	for (unsigned int i = 0; i < players.size(); i++) {
		unsigned int x = players[i].getX();
		unsigned int y = players[i].getY();
		unsigned int index = width * y + x;
		places2.at(index)->addToken(new PetriToken(petriTokenIdCounter++));
		playersPlacesPtr.push_back(places2.at(index));
		std::cout << index << std::endl;
		std::cout << "(" << players[i].getX() << ", " << players[i].getY() << ")" << std::endl;
		std::cout << "(" << playersPlacesPtr[i]->getPos().x << ", " << playersPlacesPtr[i]->getPos().y << ")" << std::endl;
	}
	for (unsigned int i = 0; i < flags.size(); i++) {
		unsigned int x = flags[i].getX();
		unsigned int y = flags[i].getY();
		unsigned int index = width * y + x;
		places2.at(index)->addToken(new PetriToken(petriTokenIdCounter++));
		flagsPlacesPtr.push_back(places2.at(index));
	}
	for (unsigned int i = 0; i < npcs.size(); i++) {
		unsigned int x = npcs[i].getX();
		unsigned int y = npcs[i].getY();
		unsigned int index = width * y + x;
		places2.at(index)->addToken(new PetriToken(petriTokenIdCounter++));
		npcsPlacesPtr.push_back(places2.at(index));
	}

	for (unsigned int i = 0; i < width; i++) {
		for (unsigned int j = 0; j < height; j++) {
			Transition* tWest = new Transition(transitionIdCounter++, false, false);
			Transition* tEast = new Transition(transitionIdCounter++, false, false);
			Transition* tNorth = new Transition(transitionIdCounter++, false, false);
			Transition* tSouth = new Transition(transitionIdCounter++, false, false);
			Transition* tNorthwest = new Transition(transitionIdCounter++, false, false);
			Transition* tNortheast = new Transition(transitionIdCounter++, false, false);
			Transition* tSouthwest = new Transition(transitionIdCounter++, false, false);
			Transition* tSoutheast = new Transition(transitionIdCounter++, false, false);

			if (i > 0) {
				Connection* connectionToWest = new Connection(connectionIdCounter++, places2.at(width * j + i), FROM_PLACE_TO_TRANSITION, 1, false);
				Connection* connectionFromWest = new Connection(connectionIdCounter++, places2.at(width * j + (i - 1)), FROM_TRANSITION_TO_PLACE, 1, false);
				tWest->addInputConnection(connectionToWest);
				tWest->addOutputConnection(connectionFromWest);
			}
			if (i < width - 1) {
				Connection* connectionToEast = new Connection(connectionIdCounter++, places2.at(width * j + i), FROM_PLACE_TO_TRANSITION, 1, false);
				Connection* connectionFromEast = new Connection(connectionIdCounter++, places2.at(width * j + (i + 1)), FROM_TRANSITION_TO_PLACE, 1, false);
				tEast->addInputConnection(connectionToEast);
				tEast->addOutputConnection(connectionFromEast);
			}
			if (j > 0) {
				Connection* connectionToNorth = new Connection(connectionIdCounter++, places2.at(width * j + i), FROM_PLACE_TO_TRANSITION, 1, false);
				Connection* connectionFromNorth = new Connection(connectionIdCounter++, places2.at(width * (j - 1) + i), FROM_TRANSITION_TO_PLACE, 1, false);
				tNorth->addInputConnection(connectionToNorth);
				tNorth->addOutputConnection(connectionFromNorth);
			}
			if (j < height - 1) {
				Connection* connectionToSouth = new Connection(connectionIdCounter++, places2.at(width * j + i), FROM_PLACE_TO_TRANSITION, 1, false);
				Connection* connectionFromSouth = new Connection(connectionIdCounter++, places2.at(width * (j + 1) + i), FROM_TRANSITION_TO_PLACE, 1, false);
				tSouth->addInputConnection(connectionToSouth);
				tSouth->addOutputConnection(connectionFromSouth);
			}

			if (i > 0 && j > 0) {
				Connection* connectionToNorthwest = new Connection(connectionIdCounter++, places2.at(width * j + i), FROM_PLACE_TO_TRANSITION, 1, false);
				Connection* connectionFromNorthwest = new Connection(connectionIdCounter++, places2.at(width * (j - 1) + (i - 1)), FROM_TRANSITION_TO_PLACE, 1, false);
				tNorthwest->addInputConnection(connectionToNorthwest);
				tNorthwest->addOutputConnection(connectionFromNorthwest);
			}
			if (i < width - 1 && j > 0) {
				Connection* connectionToNortheast = new Connection(connectionIdCounter++, places2.at(width * j + i), FROM_PLACE_TO_TRANSITION, 1, false);
				Connection* connectionFromNortheast = new Connection(connectionIdCounter++, places2.at(width * (j - 1) + (i + 1)), FROM_TRANSITION_TO_PLACE, 1, false);
				tNortheast->addInputConnection(connectionToNortheast);
				tNortheast->addOutputConnection(connectionFromNortheast);
			}
			if (i > 0 && j < height - 1) {
				Connection* connectionToSouthwest = new Connection(connectionIdCounter++, places2.at(width * j + i), FROM_PLACE_TO_TRANSITION, 1, false);
				Connection* connectionFromSouthwest = new Connection(connectionIdCounter++, places2.at(width * (j + 1) + (i - 1)), FROM_TRANSITION_TO_PLACE, 1, false);
				tSouthwest->addInputConnection(connectionToSouthwest);
				tSouthwest->addOutputConnection(connectionFromSouthwest);
			}
			if (i < width - 1 && j < height - 1) {
				Connection* connectionToSoutheast = new Connection(connectionIdCounter++, places2.at(width * j + i), FROM_PLACE_TO_TRANSITION, 1, false);
				Connection* connectionFromSoutheast = new Connection(connectionIdCounter++, places2.at(width * (j + 1) + (i + 1)), FROM_TRANSITION_TO_PLACE, 1, false);
				tSoutheast->addInputConnection(connectionToSoutheast);
				tSoutheast->addOutputConnection(connectionFromSoutheast);
			}

			transitions2.push_back(tNorthwest);
			transitions2.push_back(tWest);
			transitions2.push_back(tSouthwest);
			transitions2.push_back(tNorth);
			transitions2.push_back(tSouth);
			transitions2.push_back(tNortheast);
			transitions2.push_back(tEast);
			transitions2.push_back(tSoutheast);
		}
	}

	for (unsigned int i = 0; i < players.size(); i++) {
		unsigned int x = players[i].getX();
		unsigned int y = players[i].getY();
		unsigned int index = width * y + x;
		for (unsigned int j = 0; j < 8; j++) {
			playersInTransitionsPtr.push_back(transitions2.at(8 * index + j));
			playersOutTransitionsPtr.push_back(transitions2.at(8 * index + j));
		}
	}
	for (unsigned int i = 0; i < flags.size(); i++) {
		unsigned int x = flags[i].getX();
		unsigned int y = flags[i].getY();
		unsigned int index = width * y + x;
		for (unsigned int j = 0; j < 8; j++) {
			flagsInTransitionsPtr.push_back(transitions2.at(8 * index + j));
			flagsOutTransitionsPtr.push_back(transitions2.at(8 * index + j));
		}
	}
	for (unsigned int i = 0; i < npcs.size(); i++) {
		unsigned int x = npcs[i].getX();
		unsigned int y = npcs[i].getY();
		unsigned int index = width * y + x;
		for (unsigned int j = 0; j < 8; j++) {
			npcsInTransitionsPtr.push_back(transitions2.at(8 * index + j));
			npcsOutTransitionsPtr.push_back(transitions2.at(8 * index + j));
		}
	}
}
