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
#include "../libs/boost/algorithm/string.hpp"

using namespace std;
using json = nlohmann::json;

class Game {
private:
    /* == Attributes == */
	
	/* Lobby variables */

	// ID of the next game
	static int nextId;

	// ID of the game
	int id;

	// Name of the game room
	string name;

	// Password of the game room
	string password;

	// Maximum game capacity
	int maxPlayers;

	// Number of players in the game
	int nbPlayers;

	// Vector of all the players involved in the game
	vector <Player> players;

	// Boolean indicating if the game is running
	bool running;

	// Map of the game
	Map map;

	/* Game variables - Only available when Game is running */

	// Vector of the current state of all territories of the map
	vector <TerritoryState> board;

	// Number of free territories left
	int freeTerritories;

	// Number of player eliminations
	int eliminationCount;

	// Current game phase
	int phase;

	// ID of the active player
	int activePlayer;

	// Array of available tokens
	int tokens[4];

	// Number of total exchanged sets
	int totalExchangedSets;

	/* Turn variables - Reset after each turn */

	// Boolean indicating if a territory has been captured this turn
	bool territoryCapture; 

	// ID of the last attacking territory
	int lastAttackingTerritory;

	// ID of the last attacked territory
	int lastAttackedTerritory;

	// Boolean indicating if the last attack resulted in a capture
	bool lastAttackCapture;

	// Boolean indicating if the active player used his free move in phase 2
	bool moved;

	// Combat handler - Used to save combat context between messages, to handle combat properly - Has to be reset after each combat is solved
	CombatHandler combat;

	/* == Methods == */

	/**
	 * @brief Converts the game from its lobby state to a running game
	 *
	 */
	void start();

	/**
	 * @brief Passes the turn to the next player
	 *
	 * @return ID of the new activePlayer
	 */
	int nextPlayer();

	/**
	 * @brief Proceeds to the next phase
	 *
	 * @return int : ID of new phase
	 */
	int nextPhase();

	/**
	 * @brief Randomly chooses the first player of the game amongst the available players
	 *
	 */
	void chooseFirstPlayer();

	/**
	 * @brief Checks if activePlayer owns necessary tokens, spend them, and grants extra reinforcement
	 *
	 * @param tok1 : First token used
	 * @param tok2 : Second token used
	 * @param tok3 : Third token used
	 *
	 * @return int > 0 : Success and number of reinforcement given
	 * @return int -3 : Error, activePlayer doesn't have the necessary tokens
	 */
	int useSet(int tok1, int tok2, int tok3);

	/**
	 * @brief Calculates the number of reinforcement that would be granted for exchanging a set
	 * 
	 * @return int : Number of reinforcement that would be granted for a set
	 */
	int currentSetValue();

	/**
	 * @brief Checks if the tokens received form a valid set
	 * 
	 * @param tok1 : First token used
	 * @param tok2 : Second token used
	 * @param tok3 : Third token used
	 *
	 * @return true : The tokens received form a valid set
	 * @return false : The tokens received don't form a valid set
	 */
	bool isValidSet(int tok1, int tok2, int tok3);

	/**
	 * @brief Grants a random token from the pool to activePlayer
	 * 
	 * @return int : Type of token granted
	 */
	int grantToken();

	/**
	 * @brief Checks if activePlayer is allowed to put units on territory, and proceeds to do so
	 * 
	 * @param territory : Target destination of the reinforcement
	 * @param units : Number of units to put
	 * @return int 0 : Success, units put on the desired territory
	 * @return int -4 : Error, activePlayer doesn't own the territory, or doesn't have enough reinforcement available
	 */
	int putUnits(int territory, int units);

	/**
	 * @brief Simulates a combat and returns unit loss of involved factions
	 *
	 * @param attackers : Number of attacking units
	 * @param defenders : Number of defending units
	 *
	 * @param CombatOutcome : A CombatOutcome containing unit loss of involved factions
	 */
	CombatOutcome solveCombat(int attackers, int defenders); // Currently generates wrong probabilities for 3v2, because of the use of rand()

	/**
	 * @brief Moves units from one territory to another
	 *
	 * @param source : Territory ID of the source
	 * @param destination : Territory ID of the destination
	 * @param Number of units
	 *
	 * @return int 0 : Success, units are moved from source to destination
	 * @return int -6 : Error, action was not performed
	 */
	int moveUnits(int source, int destination, int units);

	/**
	 * @brief Calculates initial reinforcement and grant them to all players
	 *
	 * @return int : Returned value was added to each player's reinforcement
	 */
	int setInitialReinforcement();

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
	 * @brief Checks if territories `a` and `b` are neighbors
	 *
	 * @param a : Territory ID of first territory
	 * @param b : Territory ID of second territory
	 * @return True : `a` and `b` are adjacent
	 * @return False : `a` and `b` aren't adjacent
	 */
	bool areAdjacent(int a, int b);

	/**
	 * @brief Returns the ID of the owner of the continent `idContinent`
	 *
	 * @param idContinent
	 * @return int >= 0 : ID of the player if an owner exists
	 * @return int -1 : Nobody controls the continent
	 */
	int continentOwner(int idContinent);

	/**
	 * @brief Resets the CombatHandler of the game, to be able to proceed with next combat
	 * 
	 */
	void resetCombat();

	/**
	 * @brief Resets the turn variables of the game, to be able to proceed with next player
	 * 
	 */
	void resetTurnVariables();

	// Constructors

public:
    /* == Attributes == */

    /* == Methods == */

	/**
	 * @brief Calculates and grants turn reinforcement to activePlayer, which has to be set properly before calling this method
	 * 
	 * @return int : Amount of reinforcement granted
	 */
	int turnReinforcement();

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
	 * @return true
	 * @return false
	 */
	bool isActivePlayer(string name);

	/**
	 * @brief addPlayer: adds the player to the game
	 *
	 * @param name of the player
	 */
	int addPlayer(string name);

	/**
	 * @brief
	 *
	 * @param name
	 */
	int removePlayer(string name);

	/**
	 * @brief Get the player's owned territories
	 *
	 * @param int player ID
	 * @return vector of the player's territories
	 */
	vector<int> getPlayerTerritories(int player);

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
	 * @return ID of the game
	 */
	int getId();

	/**
	 * @brief Get the nbPlayers attribute
	 *
	 * @return Number of players in the game
	 */
	int getNbPlayers();

	/**
	 * @brief Get the activePlayer attribute
	 *
	 * @return ID of the active player
	 */
	int getActivePlayer();

	/**
	 * @brief Get the board attribute
	 *
	 * @return vector<TerritoryState>
	 */
	vector<TerritoryState> getGameBoard();

	/**
	 * @brief Get the Password attribute
	 *
	 * @return Password of the game
	 */
	string getPassword();

	/**
	 * @brief getPḧase : getter on phase
	 *
	 * @return current phase
	 */
	int getPhase();

	/**
	 * @brief Get the players attribute
	 *
	 * @return vector<Player>
	 */
	vector<Player> getPlayers();

	/**
	* @brief Send a message to the Game object and process it
	*
	* @return vector<json>
	*/
	vector<json> message(json message);

	/**
	 * @brief Puts information about the game in a string
	 *
	 */
	string toString();

	/**
	 * @brief Prints the status of the game
	 *
	 */
	void printGame();

	/**
	 * @brief toJson : return a json formated string of the game
	 * @return json formated string of the game
	 */
	string toJson();

	/**
	 * @brief getCombat : getter on the CombatHandler
	 * @return the CombatHandler;
	 */
	CombatHandler getCombat();

    /* == Constructors == */	

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

	// Game logic methods, names are currently placeholders
	// We'll discuss the return value later

	int messageStart();
	int messageEndPhase(int player);
	int messagePut(int player, int territory, int units);
	int messageUseTokens(int player, int token1, int token2, int token3);
	int messageAttack(int player, int source, int destination, int units);
	CombatOutcome messageDefend(int player, int units);
	int messageMove(int player, int source, int destination, int units);
};

#endif //GAME_H
