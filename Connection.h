#pragma once
#include <iostream>
#include "Place.h"

#define ARC_DIR bool
#define FROM_PLACE_TO_TRANSITION false
#define FROM_TRANSITION_TO_PLACE true

class Connection
{
public:
	Connection(unsigned int id, Place* place, ARC_DIR direction, unsigned int weight = 1, bool isInhibitor = false);
	Connection(Connection* connection);
	~Connection();
	unsigned int getId();
	void setId(unsigned int id);
	unsigned int getWeight();
	void setWeight(unsigned int weight);
	bool getIsInhibitor();
	void setIsInhibitor(bool isInhibitor);
	Place* getPlace();
	void setPlace(Place* place);
	ARC_DIR getDirection();
	void setDirection(ARC_DIR direction);
	void print();
private:
	unsigned int id;
	unsigned int weight;
	bool isInhibitor;
	Place* place;
	ARC_DIR direction;
};
