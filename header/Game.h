#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>
#include <random>
#include "Player.h"
#include "Map.h"
#include "Structs.h"
#include "MessageCode.h"

#include "../libs/json/json.hpp"

using namespace std;
using json = nlohmann::json;

class Game {
private:
	// Attributes
	// Lobby variables
	static int nextId;
	int id;
	string name;
	string password;
	int maxPlayers;
	int nbPlayers;
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
	 * @brief turnReinforcement() : gives the reinforcements that the player is entitled to 
	 * 	at the beginning of the turn
	 * 
	 */
	void turnReinforcement();

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
	void grantToken();
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

	/**
	 * @brief continentOwner : Checks if the continent is dominated by the player
	 * 
	 * @param id
	 * @param idPlayer 
	 * @return true 
	 * @return false 
	 */
	int continentOwner(int idContinent);

	// Constructors

public:
	// Attributes

	// Methods

	/**
	 * @brief toJSON: Gives information about the game instance in json 
	 * 
	 * @return json 
	 */
	json toJSON();

	/**
	 * @brief 
	 * 
	 * @return true 
	 * @return false 
	 */
	bool isRunning();

	/**
	 * @brief isFull : tells if the Gameroom is full or not
	 * 
	 * @return true 
	 * @return false 
	 */
	bool isFull();

	/**
	 * @brief 
	 * 
	 * @param name 
	 */
	int addPlayer(string name);

	/**
	 * @brief 
	 * 
	 * @param name 
	 */
	int removePlayer(string name);

	/**
	 * @brief Get the player's turn order
	 * 
	 * @param name 
	 * @return int 
	 */
	int getPlayerOrder(string name);

	/**
	 * @brief Get the Id attribute
	 * 
	 * @return int 
	 */
	int getId();

	/**
	 * @brief Get the nbPlayers attribute
	 * 
	 * @return int 
	 */
	int getNbPlayers();

	/**
	 * @brief Get the Password attribute
	 * 
	 * @return string 
	 */
	string getPassword();

	/**
	 * @brief Get the players attribute
	 * 
	 * @return vector<Player> 
	 */
	vector<Player> getPlayers();

	/**
	* @brief Send a message to the Game object and process it
	*
	* @return string
	*/
	string message(string message);

	// Constructors

	/**
	 * @brief Construct a new Game object
	 * 
	 * @param mapName 
	 * @param creatorId 
	 * @param maxPlayers 
	 */
	Game(string mapName, string creatorId, int maxPlayers);
	Game(string mapName, string creatorId, int maxPlayers, string lobbyName); // Constructor used on lobby creation
	Game(string mapName, string creatorId, int maxPlayers, string lobbyName, string password);
};

#endif //GAME_H