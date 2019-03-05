#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <vector>
#include <string>
#include "Game.h"

class GameServer {
private:
    static std::vector<Game> games;
    // field: Type ?

public:
    static int main();
    static void listen();
    static void treatMessage(std::string message);
    static void createGame(std::string mapName, std::vector<std::string> playersNames);
    static int destroyGame(int id);

};

#endif //GAMESERVER_H