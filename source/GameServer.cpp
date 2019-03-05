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
    /* create a new game */
    Game newGame(mapName, playersNames, MAX_PLAYERS);

    /* copy the new object into the games list */
    games.push_back(newGame);
}

int GameServer::destroyGame(int id)
{
    bool found = false;

    /* for each game in the games list .. */
    for (int i = 0; i < games.size(); i++)
    {
        /* .. we test the id .. */
        if (games[i].getId() == id)
        {
            /* .. and we erase it if we find it */
            games.erase(games.begin() + i);
            found = true;
        }
    }

    return found?0:-1;
}