#pragma once
#include "Connection.h"

class Transition
{
public:
	Transition(unsigned int id, bool isEnabled = false, bool verbose = false);
	Transition(Transition* transition);
	~Transition();
	unsigned int getId();
	void setId(unsigned int id);
	bool getIsEnabled();
	void setIsEnabled(bool isEnabled);
	std::vector<Connection*> getInputConnections();
	bool addInputConnection(Connection* connection);
	Connection* delInputConnection(Connection* connection);
	std::vector<Connection*> getOutputConnections();
	bool addOutputConnection(Connection* connection);
	Connection* delOutputConnection(Connection* connection);
	void print();
	void setVerbose(bool verbose);

	bool forbidden;
private:
	unsigned int id;
	bool isEnabled;
	std::vector<Connection*> inputConnections;
	std::vector<Connection*> outputConnections;
	bool verbose = false;
};
