#pragma once

#include "Player.h"
#include "Map.h"

class Game {
private:
	// Attributes
	static int nextId; // Initializing the static variable in a header doesn't seem to be possible
	int id;
	int tokens[4];
	int totalExchangedSets;
	int activePlayer;
	int phase;
	bool territoryCapture;
	bool running;
	int nbPlayer;
	Map map;
	// board : list of TerritoryState
	// players : list of Player
	int lastAttackedTerritory;
	bool lastAttackCapture;

	//Methods
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

public:
};