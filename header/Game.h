#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <string>
#include <algorithm>
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

	/**
	 * @brief 
	 * 
	 */
	void start();

	/**
	 * @brief nextPlayer : Passes the turn to the next player
	 * 
	 */
	void nextPlayer();

	/**
	 * @brief nextPhase : Goes to the next phase
	 * 
	 */
	void nextPhase();

	/**
	 * @brief 
	 * 
	 */
	void chooseFirstPlayer();

	/**
	 * @brief 
	 * 
	 * @param tok1 
	 * @param tok2 
	 * @param tok3 
	 * @return int 
	 */
	int useSet(int tok1, int tok2, int tok3);

	/**
	 * @brief 
	 * 
	 */
	void solveCombat();

	/**
	 * @brief moveUnits : Moving units from one territory to another
	 * 
	 * @param source 
	 * @param destination 
	 * @param units 
	 */
	void moveUnits(int source, int destination, int units);

	/**
	 * @brief Set the Initial Reinforcement object
	 * 
	 */
	void setInitialReinforcement();

	/**
	 * @brief 
	 * 
	 */
	void end();

	/**
	 * @brief 
	 * 
	 * @return int 
	 */
	int updatePlayersStatsInDB();

	/**
	 * @brief areAdjacent : Checks if `a` and `b` are neighbors
	 * 
	 * @param a 
	 * @param b 
	 * @return true 
	 * @return false 
	 */
	bool areAdjacent(int a, int b);

	// Constructors

	Game(string mapName, vector <string> playersNames);

public:
	// Attributes

	// Methods

	/**
	 * @brief 
	 * 
	 * @return true 
	 * @return false 
	 */
	bool isRunning();

	/**
	 * @brief 
	 * 
	 * @param name 
	 */
	void addPlayer(string name);

	/**
	 * @brief Get the Id object
	 * 
	 * @return int 
	 */
	int getId();
	
	// Constructors

	/**
	 * @brief Construct a new Game object
	 * 
	 * @param mapName 
	 * @param creatorId 
	 * @param maxPlayer 
	 */
	Game(string mapName, string creatorId, int maxPlayer); // Constructor used on lobby creation
};

#endif //GAME_H