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
	int victories;
    int defeats;
    int gained_territories;
    int lost_territories;
    int set_of_tokens[3];
    int sets_of_tokens;
    bool isAlive;
    //Player();
    Player(std::string name);
    bool set_is_valid(int tok1, int tok2, int tok3);
	void hasSet(int tok1, int tok2, int tok3);
    /*std::vector<int>listTokens(); */
    int* listTokens();
    void removeAllTokens();
    void receiveToken(int type);
    void receiveTokens(int numberOfEachToken[4]);
    void removeTokens(int numberOfEachToken[4]);
    void give_reinforcement(int number_of_rein, int last_rein_val);
    int get_victories();
    int get_defeats();
    int get_gained_territories();
    int get_lost_territories();
    int get_sets_of_tokens();
    void set_victories();
    void set_defeats();
    void set_gained_territories();
    void set_lost_territories();
    void set_sets_of_tokens();
    void die();
	void addReinforcement(int reinforcement);
	void spendReinforcement(int reinforcement);
	int getReinforcement();
    //~Player();
private:
};

#endif 
