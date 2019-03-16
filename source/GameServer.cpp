#include "../header/GameServer.h"

/* debug */

#define MAX_PLAYERS 6

/* debug */


vector<Game> GameServer::games;

ServerEndpoint GameServer::endpoint;
vector<Connection> GameServer::connections;


void GameServer::listen()
{

}


void GameServer::treatMessage(string message)
{

}


void GameServer::createGame(string mapName, vector<string> playersNames)
{
    /* create a new game */
    Game newGame(mapName, playersNames[0], MAX_PLAYERS);

    /* copy the new object into the games list */
    games.push_back(newGame);
}


int GameServer::destroyGame(int id)
{
    bool found = false;

    /* for each game in the games list .. */
    for (size_t i = 0; i < games.size() && !found; i++)
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


/* sockets */

void GameServer::init()
{
    /* Set logging settings */ 
    endpoint.set_error_channels(websocketpp::log::elevel::all); 
    endpoint.set_access_channels(websocketpp::log::alevel::all ^
        websocketpp::log::alevel::frame_payload);

    /* Initialize Asio */
    endpoint.init_asio(); 

    /* Register new message handler */
    endpoint.set_message_handler(&onMessage);
}


void GameServer::run()
{
    /* Listen on port */
    endpoint.listen(SERVER_PORT);

    /* Queues a connection accept operation */ 
    endpoint.start_accept();

    /* Start the Asio io_service run loop */
    endpoint.run(); 
}


void GameServer::onMessage(Connection connection, Message msg)
{
    cout << "Message received: " << msg->get_payload() << endl;
}


/* debugging */
int GameServer::nbGames()
{
    return games.size();
}