#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <string>
#include "Player.h"
#include "Map.h"
#include "Structs.h"
using namespace std;

class Game {
private:
	// Attributes
	static int nextId;
	int id;
	int tokens[4];
	int totalExchangedSets;
	int activePlayer;
	int phase;
	bool territoryCapture;
	bool running;
	int nbPlayer;
	int maxPlayer;
	Map map;
	vector <TerritoryState> board;
	vector <Player> players;
	int lastAttackedTerritory;
	bool lastAttackCapture;

	// Methods
	void start();
	void nextPlayer();
	void nextPhase();
	void chooseFirstPlayer();
	int useSet(int tok1, int tok2, int tok3);
	void solveCombat();
	void moveUnits(int source, int destination, int units);
	void setInitialReinforcement();
	void end();
	int updatePlayersStatsInDB();

	// Constructors
	Game(string mapName, vector <string> playersNames);

public:
	// Attributes
	// Methods
	bool isRunning();
	void addPlayer(string name);
	int getId();
	// Constructors
	Game(string mapName, string creatorId, int maxPlayer); // Constructor used on lobby creation
};

#endif //GAME_H