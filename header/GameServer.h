#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <vector>
#include <string>
#include "../header/Game.h"

class GameServer {
private:
    static std::vector<Game> games;

public:
    // static int main();
    static void listen();
    static void treatMessage(std::string message);
    static void createGame(std::string mapName, std::vector<std::string> playersNames);
    static int destroyGame(int id);

    static int nbGames() {return games.size();} 

};

#endif //GAMESERVER_H