#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include <iostream>
#include "../libs/boost/algorithm/string.hpp"

using namespace std;


class Player {
public:
	// Player status
	std::string name;
    int tokens[4];
    int reinforcement;
	bool isAlive;
	int territoriesOwned;

	// Player stats
    int territoriesCaptured;
    int territoriesLost;
	int unitsKilled;
	int unitsLost;
    int setsSpent;

    /**
     * @brief Player : Constructor
     * @param name : Player name
     */
    Player(std::string name);

    /**
     * @brief hasSet : Check if the player has a set of given tokens
     * @param tok1 : type of token #1
     * @param tok2 : type of token #2
     * @param tok3 : type of token #3
     * @return true if the player has the given set, false otherwise
     */
    bool hasSet(int tok1, int tok2, int tok3);
    /*std::vector<int>listTokens(); */

    /**
     * @brief listTokens : getter on Player's tokens
     * @return int[4] containing the number of each type of token possessed
     */
    int* listTokens();

    /**
     * @brief removeAllTokens : Set to 0 the number of each type of Player's token
     */
    void removeAllTokens();

    /**
     * @brief receiveToken : add one token of the given type to the Player
     * @param type : token's type
     */
    void receiveToken(int type);

    /**
     * @brief removeToken : remove one token of the given type to the Player
     * @param type : token's type
     */
    void removeToken(int type);

    /**
     * @brief revieveTokens : give the Player multiple tokens of different type
     * @param numberOfEachToken : number of each token (index represents the token's type)
     */
    void receiveTokens(int numberOfEachToken[4]);

    /**
     * @brief removeTokens : remove multiple tokens of different type to the Player
     * @param numberOfEachToken : number of each token (index represents the token's type)
     */
    void removeTokens(int numberOfEachToken[4]);

    /**
     * @brief getTerritoriesCaptured : getter
     * @return number of terrirories captured
     */
    int getTerritoriesCaptured();

    /**
     * @brief getTerritoriesLost : getter
     * @return number of terrirories lost
     */
    int getTerritoriesLost();
   
    /**
     * @brief getSetsSpent : getter
     * @return number of sets spent
     */
    int getSetsSpent();

    /**
     * @brief addTerritoriesCaptured : add one to the number of territories captured
     */
    void addTerritoriesCaptured();

    /**
     * @brief addTerritoriesCaptured : add one to the number of territories lost
     */
    void addTerritoriesLost();

    /**
     * @brief addSetsSpent : add one to the number of sets spent
     */
    void addSetsSpent();

    /**
     * @brief die : kill the player (isAlive = false)
     */
    void die();

    /**
     * @brief addReinforcement : add some reinforcement to the player
     * @param reinforcement : number of reinforcement to add
     */
    void addReinforcement(int reinforcement);
   
    /**
     * @brief spendReindorcement : spend some reinforcement of the player
     * @param reinforcement : number of reinforcement to spend
     */
    void spendReinforcement(int reinforcement);

    /**
     * @brief getReinforcement : getter
     * @return number of reinforcement
     */
    int getReinforcement();

    /**
     * @brief getTerritoriesOwned : getter
     * @return number of territoriesOwned
     */
    int getTerritoriesOwned();

    /**
     * @brief addTerritoriesOwned : add one to the number of territories owned by the player
     */
    void addTerritoriesOwned();

    /**
     * @brief subTerritoriesOwned : remove one to the number of territories owned by the player
     */
    void subTerritoriesOwned();

    /**
     * @brief getName : getter
     * @return player's name
     */
    string getName();

    /**
     * @brief toString: returning information about a player into a string
     * @return string 
     */
    string toString();

    /**
     * @brief printPlayer: prints the status of a players
     * 
     */
    void printPlayer();

	int countTokensOfType(int type);
	int countTokens();

private:
};

#endif 
