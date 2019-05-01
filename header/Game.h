/**
 * @file Game.h
 * @brief Class managing the whole game's logic
 */

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
	* @brief Calculates and grants turn reinforcement to activePlayer, which has to be set properly before calling this method
	*
	* @return int : Amount of reinforcement granted
	*/
	int turnReinforcement();

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
	 * @brief Checks if territories a and b are neighbors
	 *
	 * @param a : Territory ID of first territory
	 * @param b : Territory ID of second territory
	 *
	 * @return True : Territories a and b are adjacent
	 * @return False : Territories a and b aren't adjacent
	 */
	bool areAdjacent(int a, int b);

	/**
	 * @brief Returns the ID of the owner of the continent `idContinent`
	 *
	 * @param idContinent : ID of the continent to check for ownership
	 *
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
	 * @brief toJSON: Gives information about the game instance in json
	 *
	 * @return json
	 */
	json toJSON();

	/**
	 * @brief Checks if the Game currently functions as a lobby or a running game
	 *
	 * @return true : Game is currently running
	 * @return false : Game is currently not running and functions as a lobby
	 */
	bool isRunning();

	/**
	 * @brief Checks if the lobby is full
	 *
	 * @return true : Lobby is full and can't allow more players
	 * @return false : Lobby is not full and can allow more players
	 */
	bool isFull();

	/**
	 * @brief Checks if the player name passed as parameter matches the ID of the current active player
	 *
	 * @return true : Named player is currently the active player
	 * @return false : Named player is currently not the active player
	 */
	bool isActivePlayer(string name);

	/**
	 * @brief Attempts to add the player to the game
	 *
	 * @param name : Name of the player to add
	 *
	 * @return int 0 : Success, named player was added to the game and created as a player entity
	 * @return int -1 : Error, a new player couldn't be added to the game
	 */
	int addPlayer(string name);

	/**
	 * @brief Attempts to remove the player from the game
	 *
	 * @param name : Name of the player to remove
	 *
	 * @return int 0 : Success, named player was removed from the game
	 * @return int -1 : Error, named player didn't exist in the game and no action was performed
	 */
	int removePlayer(string name);

	/**
	 * @brief Gets the player's owned territories
	 *
	 * @param player : ID of player to check
	 * @return vector<int> : Vector containing the ID of all the territories owned by player
	 */
	vector<int> getPlayerTerritories(int player);

	/**
	 * @brief Gets the player's turn order
	 *
	 * @param name : Name of the player to check
	 *
	 * @return int >= 0 : Order of the named player
	 * @return int -1 : Named player wasn't found in the game
	 */
	int getPlayerOrder(string name);

	/**
	 * @brief Gets the ID attribute of the game
	 *
	 * @return int : ID of the game
	 */
	int getId();

	/**
	 * @brief Gets the nbPlayers attribute
	 *
	 * @return int : Number of players in the game
	 */
	int getNbPlayers();

	/**
	 * @brief Gets the activePlayer attribute
	 *
	 * @return int : ID of the current active player
	 */
	int getActivePlayer();

	/**
	 * @brief Gets the board attribute, containing the current state of all territories
	 *
	 * @return vector<TerritoryState> : Vector containing the current state of all territories
	 */
	vector<TerritoryState> getGameBoard();

	/**
	 * @brief Gets the Password attribute
	 *
	 * @return string : Password of the game
	 */
	string getPassword();

	/**
	 * @brief Gets the current phase
	 *
	 * @return int : Current phase
	 */
	int getPhase();

	/**
	 * @brief Gets the players attribute, containing all players involved in the game
	 *
	 * @return vector<Player> : Vector containing all players involved in the game
	 */
	vector<Player> getPlayers();

	/**
	 * @brief Puts information about the game in a string
	 *
	 * @return string : Strong containing all game info
	 */
	string toString();

	/**
	 * @brief Prints the status of the game
	 *
	 */
	void printGame();

	/**
	 * @brief Returns a json formatted string of the game
	 *
	 * @return json : Formatted string of the game
	 */
	string toJson();

	/**
	 * @brief Gets the CombatHandler
	 *
	 * @return CombatHandler : current state of the game's CombatHandler
	 */
	CombatHandler getCombat();

    /* == Constructors == */	

	/**
	 * @brief Constructs a new Game object
	 *
	 * @param mapName : Name of the desired map
	 * @param creatorId : Name of the creator of the game, whom will act as first player
	 * @param maxPlayers : Desired maximum number of players in the game, limited by the map's own capacity
	 *
	 * @return Game : Reference to the game created
	 */
	Game(string mapName, string creatorId, int maxPlayers);

	/**
	 * @brief Constructs a new Game object
	 *
	 * @param mapName : Name of the desired map
	 * @param creatorId : Name of the creator of the game, whom will act as first player
	 * @param maxPlayers : Desired maximum number of players in the game, limited by the map's own capacity
	 * @param lobbyName : Name of the lobby created
	 * 
	 * @return Game : Reference to the game created
	 */
	Game(string mapName, string creatorId, int maxPlayers, string lobbyName); // Constructor used on lobby creation

	/**
	 * @brief Constructs a new Game object
	 *
	 * @param mapName : Name of the desired map
	 * @param creatorId : Name of the creator of the game, whom will act as first player
	 * @param maxPlayers : Desired maximum number of players in the game, limited by the map's own capacity
	 * @param lobbyName : Name of the lobby created
	 * @param password : Password used for the lobby
	 *
	 * @return Game : Reference to the game created
	 */
	Game(string mapName, string creatorId, int maxPlayers, string lobbyName, string password);

	/* == Core game logic methods == */
	// Those methods are called when receiving messages from players, and are necessary to proceed with the game

	/**
	* @brief Starts the game and makes it start acting as a running game rather than a lobby
	*
	* @return int : ID of the player chosen as first player
	*/
	int messageStart();

	/**
	* @brief Ends current phase
	*
	* @param player : ID of the player that tried to send this message
	*
	* @return int 0 : Success, game proceeded to a new phase
	* @return int >0 : Success, new game phase is 0, and the returned number is the amount of reinforcement of the new active player
	* @return int -1 : Error, player does not match the current active player
	* @return int -2 : Error, active player must spend all remaining reinforcement
	* @return int -3 : Error, active player must spend tokens to have 4 or less left
	* @return int -4 : Error, a combat must be resolved
	*/
	int messageEndPhase(int player);

	/**
	* @brief Puts a number of units on desired territory
	*
	* @param player : ID of the player that tried to send this message
	* @param territory : ID of the territory to place units on
	* @param units : Number of units to put on territory
	*
	* @return int 0 : Success, units were successfully placed on territory
	* @return int >0 : Success, units were successfully placed on territory, new game phase is 0, and the returned number is the amount of reinforcement of the new active player
	* @return int -1 : Error, player does not match the current active player, or the message was sent during the wrong phase
	* @return int -2 : Error, in phase -1, units must always be 1
	* @return int -3 : Error, there are free territories remaining (in phase -1, they must all be occupied before reinforcing the others)
	* @return int -4 : Error, active player doesn't own the territory, or doesn't have enough reinforcement to put that number on it
	*/
	int messagePut(int player, int territory, int units);

	/**
	* @brief Spends tokens to get reinforcement
	*
	* @param player : ID of the player that tried to send this message
	* @param token1 : First token used
	* @param token2 : Second token used
	* @param token3 : Third token used
	*
	* @return int >0 : Success, number of reinforcement received
	* @return int -1 : Error, player does not match the current active player
	* @return int -2 : Error, message was sent during the wrong phase
	* @return int -3 : Error, active player doesn't own the required tokens
	* @return int -4 : Error, tokens used do not form a valid set
	*/
	int messageUseTokens(int player, int token1, int token2, int token3);

	/**
	* @brief Attacks an ennemy territory
	*
	* @param player : ID of the player that tried to send this message
	* @param source : ID of the territory to launch the attack from
	* @param destination : ID of the territory to attack
	* @param units : Number of units to send to attack
	*
	* @return int 0 : Success, combat is allowed and waiting to be resolved
	* @return int -1 : Error, player does not match the current active player
	* @return int -2 : Error, message was sent during the wrong phase
	* @return int -3 : Error, invalid range of units used for a combat
	* @return int -4 : Error, a combat must be resolved
	* @return int -5 : Error, active player doesn't own the source territory
	* @return int -6 : Error, active player owns the territory he's trying to attack
	* @return int -7 : Error, source and destination are not adjacent territories
	* @return int -8 : Error, active player doesn't have enough available units on the attacking territory
	*/
	int messageAttack(int player, int source, int destination, int units);

	/**
	* @brief Defends an attacked territory
	*
	* @param player : ID of the player that tried to send this message
	* @param units : Number of units to use to defend
	*
	* @return CombatOutcome : Structure containing all the necessary info about the solved combat, check outcomeType for actual return value
	* @return int outcomeType 0 : Success, CombatOutcome contains unit loss and involved territories
	* @return int outcomeType 1 : Success, same as 0, but the defending player was eliminated, granting additional tokens to active player
	* @return int outcomeType 2 : Success, the last remaining player was eliminated, resulting of the victory of active player
	* @return int outcomeType -1 : Error, player does not match the current active player
	* @return int outcomeType -2 : Error, no combat requires solving
	* @return int outcomeType -3 : Error, message was sent during the wrong phase
	* @return int outcomeType -4 : Error, invalid range of units used for a combat
	* @return int outcomeType -5 : Error, defending player doesn't have enough available units on the defending territory
	*/
	CombatOutcome messageDefend(int player, int units);

	/**
	* @brief Moves units between two allied territories
	*
	* @param player : ID of the player that tried to send this message
	* @param source : ID of the territory to move the units from
	* @param destination : ID of the territory to move the units to
	* @param units : Number of units to move
	*
	* @return int 0 : Success, units are moved from source to destination
	* @return int -1 : Error, player does not match the current active player
	* @return int -2 : Error, message was sent during the wrong phase
	* @return int -3 : Error, in phase 1, the last attack wasn't a capture
	* @return int -4 : Error, in phase 1, territories were not involved in the last capture
	* @return int -5 : Error, in phase 2, active player already used his free move
	* @return int -6 : Error, invalid move
	*/
	int messageMove(int player, int source, int destination, int units);

	/**
	* @brief Simulates end game by putting the game in a state close to the end
	*
	* @param password : Send 34 to allow the method to proceed
	*
	* @return ? : Full game status to be sent to all players
	*/
	void adminEndGameSimulation(int password);
};

#endif //GAME_H
