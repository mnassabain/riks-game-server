#include "../header/GameServer.h"

/* debug */

#define MAX_PLAYERS 6

/* debug */


std::vector<Game> GameServer::games;

int GameServer::main()
{
    return 0;
}

void GameServer::listen()
{

}

void GameServer::treatMessage(std::string message)
{

}

void GameServer::createGame(std::string mapName, std::vector<std::string> playersNames)
{
    Game newGame(mapName, playersNames, MAX_PLAYERS);
    games.push_back(newGame);
}

int GameServer::destroyGame(int id)
{
    return 0;
}