#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>

class Player {
public:
	std::string name;
	/*std::vector<int> tokens = std::vector<int>(4) */
    int tokens[4];
    int reinforcement;
    int gained_territories;
    int lost_territories;
    int sets_of_tokens;
    bool isAlive;
    Player(std::string name);
	bool hasSet(int tok1, int tok2, int tok3);
    /*std::vector<int>listTokens(); */
    int* listTokens();
    void removeAllTokens();
    void receiveToken(int type);
    void receiveTokens(int numberOfEachToken[4]);
    void removeTokens(int numberOfEachToken[4]);
    int get_victories();
    int get_defeats();
    int get_gained_territories();
    int get_lost_territories();
    int get_sets_of_tokens();
    void add_gained_territories();
    void add_lost_territories();
    void add_sets_of_tokens();
    void die();
	void addReinforcement(int reinforcement);
	void spendReinforcement(int reinforcement);
	int getReinforcement();
private:
};

#endif 
