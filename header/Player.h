#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>

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

    Player(std::string name);
	bool hasSet(int tok1, int tok2, int tok3);
    /*std::vector<int>listTokens(); */
    int* listTokens();
    void removeAllTokens();
    void receiveToken(int type);
	void removeToken(int type);
    void receiveTokens(int numberOfEachToken[4]);
    void removeTokens(int numberOfEachToken[4]);
    int getTerritoriesCaptured();
    int getTerritoriesLost();
    int getSetsSpent();
    void addTerritoriesCaptured();
    void addTerritoriesLost();
    void addSetsSpent();
    void die();

	void addReinforcement(int reinforcement);
	void spendReinforcement(int reinforcement);
	int getReinforcement();
	int getTerritoriesOwned();
	void addTerritoriesOwned();
	void subTerritoriesOwned();

	int countTokensOfType(int type);
	int countTokens();

	std::string getName();
private:
};

#endif 
