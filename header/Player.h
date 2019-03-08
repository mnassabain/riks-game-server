#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>

class Player {
public:
	std::string name;
	std::vector<int> tokens = std::vector<int>(4);
    int reinforcement;
	int victories;
    int defeats;
    int gained_territories;
    int lost_territories;
    int set_of_tokens[3];
    bool isAlive;
    //Player();
    Player(std::string name);
    bool set_is_valid(int tok1, int tok2, int tok3);
	void hasSet(int tok1, int tok2, int tok3);
    std::vector<int>listTokens();
    void removeAllTokens();
    void receiveToken(int type);
    void receiveTokens(int numberOfEachToken[4]);
    void removeTokens(int numberOfEachToken[4]);
    void give_reinforcement(int number_of_rein, int last_rein_val);
    //~Player();
private:
};

#endif 
