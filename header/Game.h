#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>
#include "Player.h"
#include "Map.h"
#include "Structs.h"
using namespace std;

class Game {
private:
	// Attributes
	// Lobby variables // Gotta remember to add a string name for the lobby and change the public constructor to reflect the change
	static int nextId;
	int id;
	string name;
	string password;
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
	// Turn variables - Reset after each turn
	bool territoryCapture;
	int lastAttackedTerritory;
	bool lastAttackCapture;
	// Combat handler - Used to save combat context between messages, to handle combat properly - Has to be reset after each combat is done
	CombatHandler combat;

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

	int currentSetValue();
	bool isValidSet(int tok1, int tok2, int tok3);
	void putUnits(int territory, int units);

	/**
	 * @brief 
	 * 
	 */
	CombatOutcome solveCombat(int attackers, int defenders);

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
	Game(string mapName, string creatorId, int maxPlayer);
	Game(string mapName, string creatorId, int maxPlayer, string lobbyName); // Constructor used on lobby creation
};

#endif //GAME_H