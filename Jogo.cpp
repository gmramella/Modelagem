#include "Jogo.h"

void fin(PetriToken token, Place place) {
#ifdef _WIN32
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console, BG_GREEN);
	std::cout << "Token " << token.getId() << " entered place " << place.getId() << std::endl;
	SetConsoleTextAttribute(console, FG_WHITE);
#endif
#ifdef unix
	std::cout << BG_GREEN << "Token " << token.getId() << " entered place " << place.getId() << std::endl << "\033[0m";
#endif
}

void fout(PetriToken token, Place place) {
#ifdef _WIN32
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console, BG_RED);
	std::cout << "Token " << token.getId() << " left place " << place.getId() << std::endl;
	SetConsoleTextAttribute(console, FG_WHITE);
#endif
#ifdef unix
	std::cout << BG_RED << "Token " << token.getId() << " left place " << place.getId() << std::endl << "\033[0m";
#endif
}

void fte(Transition transition) {
#ifdef _WIN32
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console, BG_YELLOW);
	std::cout << "Transition " << transition.getId() << " was enabled" << std::endl;
	SetConsoleTextAttribute(console, FG_WHITE);
#endif
#ifdef unix
	std::cout << BG_YELLOW << "Transition " << transition.getId() << " was enabled" << std::endl << "\033[0m";
#endif
}

void ftf(Transition transition) {
#ifdef _WIN32
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console, BG_BLUE);
	std::cout << "Transition " << transition.getId() << " fired" << std::endl;
	SetConsoleTextAttribute(console, FG_WHITE);
#endif
#ifdef unix
	std::cout << BG_BLUE << "Transition " << transition.getId() << " fired" << std::endl << "\033[0m";
#endif
}

Graph Jogo::gridToGraph(GridGenerator gg)
{
	GridTile** grid = gg.getGrid();

	Graph graph;
	for (unsigned int y = 0; y < height; y++) {
		for (unsigned int x = 0; x < width; x++) {
			if (grid[x][y].type != tWall)
				graph.addNode(width * y + x);
		}
	}
	std::cout << graph.getNodes().size() << " nodes" << std::endl;
	//graph.print();

	std::vector<Node> nodes = graph.getNodes();
	for (unsigned int y = 0; y < height; y++) {
		std::cout << y << std::endl;
		for (unsigned int x = 0; x < width; x++) {
			if (x > 0 && grid[x - 1][y].type != tWall) {
				//graph.addEdge(width * y + x, width * y + (x - 1), (unsigned int)1);
				//std::cout << "esq" << std::endl;
			}
			if (x < width - 1 && grid[x + 1][y].type != tWall) {
				//graph.addEdge(width * y + x, width * y + (x + 1), (unsigned int)1);
				//std::cout << "dir" << std::endl;
			}
			if (y > 0 && grid[x][y - 1].type != tWall) {
				//graph.addEdge(width * y + x, width * (y - 1) + x, (unsigned int)1);
				//std::cout << "top" << std::endl;
			}
			if (y < height - 1 && grid[x][y + 1].type != tWall) {
				//graph.addEdge(width * y + x, width * (y + 1) + x, (unsigned int)1);
				//std::cout << "down" << std::endl;
			}
		}
	}
	
	//graph.print();
	return graph;
}

PetriNet Jogo::gridToPetriNet(GridGenerator grid) {
	std::vector<Player> players = grid.getPlayers();
	std::vector<Flag> flags = grid.getFlags();
	std::vector<Npc> npcs = grid.getNpcs();

	unsigned int placeIdCounter = 0;
	unsigned int petriTokenIdCounter = 0;
	unsigned int transitionIdCounter = 0;
	unsigned int connectionIdCounter = 0;

	PetriNet pn(width, height, PETRI_NET_VERBOSE);
	pn.setTokenEnteringPlaceCallback(fin);
	pn.setTokenLeavingPlaceCallback(fout);
	pn.setTransitionEnableCallback(fte);
	pn.setTransitionFireCallback(ftf);

	Place** places = new Place*[width];
	for (unsigned int i = 0; i < width; i++)
		places[i] = new Place[height];

	for (unsigned int i = 0; i < width; i++) {
		for (unsigned int j = 0; j < height; j++) {
			places[i][j].setId(placeIdCounter++);
			places[i][j].setPos(Pos(i, j));
			places[i][j].setVerbose(PLACE_VERBOSE);
		}
	}

	for (unsigned int i = 0; i < players.size(); i++) {
		unsigned int x = players[i].getX();
		unsigned int y = players[i].getY();
		places[x][y].addToken(new PetriToken(petriTokenIdCounter++));
		pn.playersPlacesPtr.push_back(&places[x][y]);
	}
	/*for (unsigned int i = 0; i < flags.size(); i++) {
		unsigned int x = flags[i].getX();
		unsigned int y = flags[i].getY();
		places[x][y].addToken(new PetriToken(petriTokenIdCounter++));
		pn.flagsPlacesPtr.push_back(&places[x][y]);
	}*/
	for (unsigned int i = 0; i < npcs.size(); i++) {
		unsigned int x = npcs[i].getX();
		unsigned int y = npcs[i].getY();
		places[x][y].addToken(new PetriToken(petriTokenIdCounter++));
		pn.npcsPlacesPtr.push_back(&places[x][y]);
	}

	for (unsigned int i = 0; i < width; i++) {
		for (unsigned int j = 0; j < height; j++) {
			pn.addPlace(&places[i][j]);
		}
	}
	
	for (unsigned int i = 0; i < width; i++) {
		for (unsigned int j = 0; j < height; j++) {
			Transition* tWest = new Transition(transitionIdCounter++, false, TRANSITION_VERBOSE);
			Transition* tEast = new Transition(transitionIdCounter++, false, TRANSITION_VERBOSE);
			Transition* tNorth = new Transition(transitionIdCounter++, false, TRANSITION_VERBOSE);
			Transition* tSouth = new Transition(transitionIdCounter++, false, TRANSITION_VERBOSE);
			Transition* tNorthwest = new Transition(transitionIdCounter++, false, TRANSITION_VERBOSE);
			Transition* tNortheast = new Transition(transitionIdCounter++, false, TRANSITION_VERBOSE);
			Transition* tSouthwest = new Transition(transitionIdCounter++, false, TRANSITION_VERBOSE);
			Transition* tSoutheast = new Transition(transitionIdCounter++, false, TRANSITION_VERBOSE);

			if (i > 0) {
				Connection* connectionToWest = new Connection(connectionIdCounter++, &places[i][j], FROM_PLACE_TO_TRANSITION, 1, false);
				Connection* connectionFromWest = new Connection(connectionIdCounter++, &places[i - 1][j], FROM_TRANSITION_TO_PLACE, 1, false);
				tWest->addInputConnection(connectionToWest);
				tWest->addOutputConnection(connectionFromWest);
			}
			if (i < width - 1) {
				Connection* connectionToEast = new Connection(connectionIdCounter++, &places[i][j], FROM_PLACE_TO_TRANSITION, 1, false);
				Connection* connectionFromEast = new Connection(connectionIdCounter++, &places[i + 1][j], FROM_TRANSITION_TO_PLACE, 1, false);
				tEast->addInputConnection(connectionToEast);
				tEast->addOutputConnection(connectionFromEast);
			}
			if (j > 0) {
				Connection* connectionToNorth = new Connection(connectionIdCounter++, &places[i][j], FROM_PLACE_TO_TRANSITION, 1, false);
				Connection* connectionFromNorth = new Connection(connectionIdCounter++, &places[i][j - 1], FROM_TRANSITION_TO_PLACE, 1, false);
				tNorth->addInputConnection(connectionToNorth);
				tNorth->addOutputConnection(connectionFromNorth);
			}
			if (j < height - 1) {
				Connection* connectionToSouth = new Connection(connectionIdCounter++, &places[i][j], FROM_PLACE_TO_TRANSITION, 1, false);
				Connection* connectionFromSouth = new Connection(connectionIdCounter++, &places[i][j + 1], FROM_TRANSITION_TO_PLACE, 1, false);
				tSouth->addInputConnection(connectionToSouth);
				tSouth->addOutputConnection(connectionFromSouth);
			}

			if (i > 0 && j > 0) {
				Connection* connectionToNorthwest = new Connection(connectionIdCounter++, &places[i][j], FROM_PLACE_TO_TRANSITION, 1, false);
				Connection* connectionFromNorthwest = new Connection(connectionIdCounter++, &places[i - 1][j - 1], FROM_TRANSITION_TO_PLACE, 1, false);
				tNorthwest->addInputConnection(connectionToNorthwest);
				tNorthwest->addOutputConnection(connectionFromNorthwest);
			}
			if (i < width - 1 && j > 0) {
				Connection* connectionToNortheast = new Connection(connectionIdCounter++, &places[i][j], FROM_PLACE_TO_TRANSITION, 1, false);
				Connection* connectionFromNortheast = new Connection(connectionIdCounter++, &places[i + 1][j - 1], FROM_TRANSITION_TO_PLACE, 1, false);
				tNortheast->addInputConnection(connectionToNortheast);
				tNortheast->addOutputConnection(connectionFromNortheast);
			}
			if (i > 0 && j < height - 1) {
				Connection* connectionToSouthwest = new Connection(connectionIdCounter++, &places[i][j], FROM_PLACE_TO_TRANSITION, 1, false);
				Connection* connectionFromSouthwest = new Connection(connectionIdCounter++, &places[i - 1][j + 1], FROM_TRANSITION_TO_PLACE, 1, false);
				tSouthwest->addInputConnection(connectionToSouthwest);
				tSouthwest->addOutputConnection(connectionFromSouthwest);
			}
			if (i < width - 1 && j < height - 1) {
				Connection* connectionToSoutheast = new Connection(connectionIdCounter++, &places[i][j], FROM_PLACE_TO_TRANSITION, 1, false);
				Connection* connectionFromSoutheast = new Connection(connectionIdCounter++, &places[i + 1][j + 1], FROM_TRANSITION_TO_PLACE, 1, false);
				tSoutheast->addInputConnection(connectionToSoutheast);
				tSoutheast->addOutputConnection(connectionFromSoutheast);
			}

			pn.addTransition(tNorthwest);
			pn.addTransition(tWest);
			pn.addTransition(tSouthwest);
			pn.addTransition(tNorth);
			pn.addTransition(tSouth);
			pn.addTransition(tNortheast);
			pn.addTransition(tEast);
			pn.addTransition(tSoutheast);
		}
	}

	std::vector<Transition*> transitions = pn.getTransitions();
	for (std::vector<Transition*>::iterator transition = transitions.begin(); transition != transitions.end(); transition++) {
		std::vector<Connection*> inputConnections = (*transition)->getInputConnections();
		for (std::vector<Connection*>::iterator inputConnection = inputConnections.begin(); inputConnection != inputConnections.end(); inputConnection++) {
			Place* place = (*inputConnection)->getPlace();
			for (std::vector<Place*>::iterator playerPlace = pn.playersPlacesPtr.begin(); playerPlace != pn.playersPlacesPtr.end(); playerPlace++) {
				if ((place) == (*playerPlace)) {
					pn.playersInTransitionsPtr.push_back(*transition);
				}
			}
			for (std::vector<Place*>::iterator flagPlace = pn.flagsPlacesPtr.begin(); flagPlace != pn.flagsPlacesPtr.end(); flagPlace++) {
				if ((place) == (*flagPlace)) {
					pn.flagsInTransitionsPtr.push_back(*transition);
				}
			}
			for (std::vector<Place*>::iterator npcPlace = pn.npcsPlacesPtr.begin(); npcPlace != pn.npcsPlacesPtr.end(); npcPlace++) {
				if ((place) == (*npcPlace)) {
					pn.npcsInTransitionsPtr.push_back(*transition);
				}
			}
		}
		std::vector<Connection*> outputConnections = (*transition)->getOutputConnections();
		for (std::vector<Connection*>::iterator outputConnection = outputConnections.begin(); outputConnection != outputConnections.end(); outputConnection++) {
			Place* place = (*outputConnection)->getPlace();
			for (std::vector<Place*>::iterator playerPlace = pn.playersPlacesPtr.begin(); playerPlace != pn.playersPlacesPtr.end(); playerPlace++) {
				if ((place) == (*playerPlace)) {
					pn.playersOutTransitionsPtr.push_back(*transition);
				}
			}
			for (std::vector<Place*>::iterator flagPlace = pn.flagsPlacesPtr.begin(); flagPlace != pn.flagsPlacesPtr.end(); flagPlace++) {
				if ((place) == (*flagPlace)) {
					pn.flagsOutTransitionsPtr.push_back(*transition);
				}
			}
			for (std::vector<Place*>::iterator npcPlace = pn.npcsPlacesPtr.begin(); npcPlace != pn.npcsPlacesPtr.end(); npcPlace++) {
				if ((place) == (*npcPlace)) {
					pn.npcsOutTransitionsPtr.push_back(*transition);
				}
			}
		}
	}

	std::cout << pn.playersPlacesPtr.size() << std::endl;
	std::cout << pn.flagsPlacesPtr.size() << std::endl;
	std::cout << pn.npcsPlacesPtr.size() << std::endl;

	std::cout << pn.playersInTransitionsPtr.size() << std::endl;
	std::cout << pn.flagsInTransitionsPtr.size() << std::endl;
	std::cout << pn.npcsInTransitionsPtr.size() << std::endl;

	std::cout << pn.playersOutTransitionsPtr.size() << std::endl;
	std::cout << pn.flagsOutTransitionsPtr.size() << std::endl;
	std::cout << pn.npcsOutTransitionsPtr.size() << std::endl;

	pn.showStatus();

	return pn;
}

Jogo::Jogo()
{
}

Jogo::~Jogo()
{
}

void testDijkstra() {
	Graph graph;

	/*for (size_t i = 0; i < 7500; i++)
	{
		graph.addNode(i);
	}

	srand((unsigned int)time(NULL));
	for (size_t i = 0; i < 100 * 75 * 4 - 2 * 100 - 2 * 75; i++)
	{
		graph.addEdge(rand() % 7500, rand() % 7500, 1);
	}*/

	graph.addNode(0);
	graph.addNode(1);
	graph.addNode(2);
	graph.addNode(3);
	graph.addNode(4);
	graph.addNode(5);
	graph.addNode(6);
	graph.addNode(7);
	graph.addNode(8);

	graph.addEdge(0, 1, 4);
	graph.addEdge(0, 7, 8);
	
	graph.addEdge(1, 0, 4);
	graph.addEdge(1, 2, 8);
	graph.addEdge(1, 7, 11);
	
	graph.addEdge(2, 1, 8);
	graph.addEdge(2, 3, 7);
	graph.addEdge(2, 5, 4);
	graph.addEdge(2, 8, 2);
	
	graph.addEdge(3, 2, 7);
	graph.addEdge(3, 4, 9);
	graph.addEdge(3, 5, 14);

	graph.addEdge(4, 3, 9);
	graph.addEdge(4, 5, 10);
	
	graph.addEdge(5, 2, 4);
	graph.addEdge(5, 3, 14);
	graph.addEdge(5, 4, 10);
	graph.addEdge(5, 6, 2);
	
	graph.addEdge(6, 5, 2);
	graph.addEdge(6, 7, 1);
	graph.addEdge(6, 8, 6);
	
	graph.addEdge(7, 0, 8);
	graph.addEdge(7, 1, 11);
	graph.addEdge(7, 6, 1);
	graph.addEdge(7, 8, 7);
	
	graph.addEdge(8, 2, 2);
	graph.addEdge(8, 6, 6);
	graph.addEdge(8, 7, 7);
	
	graph.print();

	std::vector<unsigned int> path = graph.dijkstra(0, 4);
	for (std::vector<unsigned int>::iterator it = path.begin(); it != path.end(); it++)
		std::cout << *it << std::endl;
}

void Jogo::inicializar()
{
	unsigned int screenWidth = 800;
	unsigned int screenHeight = 600;
	uniInicializar(screenWidth, screenHeight, false);

	gRecursos.carregarSpriteSheet("empty", "assets/empty.png", 0, 1);
	gRecursos.carregarSpriteSheet("wall", "assets/wall.png", 0, 1);
	gRecursos.carregarSpriteSheet("player", "assets/player.png", 0, 1);
	gRecursos.carregarSpriteSheet("npc", "assets/npc.png", 0, 1);
	gRecursos.carregarSpriteSheet("flag", "assets/flag.png", 0, 1);
	gRecursos.carregarSpriteSheet("flooded", "assets/flooded.png", 0, 1);

	unsigned int w = screenWidth / gRecursos.getSpriteSheet("empty")->getTextura()->getLargura();
	unsigned int h = screenHeight / gRecursos.getSpriteSheet("empty")->getTextura()->getAltura();

	TerrainGenerator tg(w, h);
	bool** terrain = tg.generate();
	
	grid.generate(terrain, w, h);
	unsigned int playersQuadrant = grid.addPlayers();
	grid.flood(tEmpty, tFlooded);
	grid.addFlags(playersQuadrant, 3);
	grid.addNpcs(playersQuadrant, 3);
	grid.fill();
	grid.evaporate();
	grid.setSpritesheets();

	//testDijkstra();

		//Graph graph = gridToGraph(grid);
	//graph.print();
	//std::vector<unsigned int> path = graph.dijkstra(0, 1);
	
	//pn = gridToPetriNet(grid);

	pn.generate(grid, width, height);
}

void Jogo::finalizar()
{
	uniFinalizar();
}

void Jogo::executar()
{
	while(!gTeclado.soltou[TECLA_ESC] && !gEventos.sair)
	{
		uniIniciarFrame();

		std::cout << pn.playersPlacesPtr.size() << std::endl;
		std::vector<Movement> playersMovements = grid.update();
		std::vector<Player> players = grid.getPlayers();
		pn.runCycle(playersMovements, players);
		grid.draw();
		gEventos.sair = (grid.getFlags().size() == 0);
		
		uniTerminarFrame();
	}
}
