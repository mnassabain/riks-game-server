#ifndef GAME_H
#define GAME_H

#include <iostream>
#include "Player.h"
#include "Map.h"
#include "Structs.h"

class Game {
private:
	// Attributes
	// Lobby variables
	static int nextId;
	int id;
	int maxPlayer;
	int nbPlayer;
	vector <Player> players;
	bool running;
	Map map;
	// Game variables - Only available when Game is running
	vector <TerritoryState> board;
	int phase;
	int activePlayer;
	int tokens[4];
	int totalExchangedSets;
	bool territoryCapture;
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