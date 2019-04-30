#pragma once
#include "libUnicornio.h"
#include "TerrainGenerator.h"
#include "GridGenerator.h"
#include "PetriToken.h"
#include "Place.h"
#include "Transition.h"
#include "Connection.h"
#include "PetriNet.h"
#include "Graph.h"
#include <Windows.h>

#ifdef _WIN32
#include <windows.h>
#define FG_BLACK		0x0000
#define FG_BLUE			0x0001
#define FG_GREEN		0x0002
#define FG_CYAN			0x0003
#define FG_RED			0x0004
#define FG_MAGENTA		0x0005
#define FG_YELLOW		0x0006
#define FG_WHITE		0x0007
#define FG_INTENSITY	0x0008
#define BG_BLUE			0x0010
#define BG_GREEN		0x0020
#define BG_CYAN			0x0030
#define BG_RED			0x0040
#define BG_MAGENTA		0x0050
#define BG_YELLOW		0x0060
#define BG_WHITE		0x0070
#define BG_INTENSITY	0x0080
#endif
//https://gist.github.com/avelino/3188137
#ifdef unix
#define FG_BLACK		"\033[0;30m"
#define FG_RED			"\033[0;31m"
#define FG_GREEN		"\033[0;32m"
#define FG_YELLOW		"\033[0;33m"
#define FG_BLUE			"\033[0;34m"
#define FG_MAGENTA		"\033[0;35m"
#define FG_CYAN			"\033[0;36m"
#define FG_WHITE		"\033[0;37m"
#define FG_INTENSITY	"\033[1;30m"
#define BG_RED			"\033[1;31m"
#define BG_GREEN		"\033[1;32m"
#define BG_YELLOW		"\033[1;33m"
#define BG_BLUE			"\033[1;34m"
#define BG_MAGENTA		"\033[1;35m"
#define BG_WHITE		"\033[1;36m"
#define BG_INTENSITY	"\033[1;37m"
#endif

#define PETRI_NET_VERBOSE false
#define PLACE_VERBOSE false
#define TRANSITION_VERBOSE false

class Jogo
{
public:
	Jogo();
	~Jogo();
	void inicializar();
	void finalizar();
	void executar();

	Graph gridToGraph(GridGenerator gg);
	PetriNet gridToPetriNet(GridGenerator grid);
private:
	GridGenerator grid;
	unsigned int width = 100;
	unsigned int height = 75;
	PetriNet pn;
};
