#include "../header/GameServer.h"

/* debug */
#define MAX_PLAYERS 6



vector<Game> GameServer::games;
ServerEndpoint GameServer::endpoint;
vector<Connection> GameServer::connections;


void GameServer::listen()
{
    init();
    run();
}


void GameServer::treatMessage(string message)
{
    json jmessage = json::parse(message);

    MessageCode code = jmessage[0];
    switch(code)
    {
        case CODE_SIGN_UP:
            cout << "Creating new user (id = " << jmessage[1]
                << ", password = " << jmessage[2] << ")" << endl;
            /* insert into db */
            break;

        case CODE_CONNECT:
            cout << "Connection attempt by user (id = " << jmessage[1]
                << ", password = " << jmessage[2] << ")" << endl;
            /* ... */
            break;

        case CODE_DISCONNECT:
            cout << "User disconnected (id = " << jmessage[1] << ")" << endl;
            break;

        case CODE_ERROR:
            cout << "Error" << endl;
            break;

        default:
            cout << "Unhandled message code" << endl;
    }
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
    for (unsigned int i = 0; i < games.size() && !found; i++)
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


/* PRIVATE METHODS */

void GameServer::init()
{
    try
    {
        /* reuse 9002 address */
        endpoint.set_reuse_addr(true);

        /* Set logging settings */ 
        endpoint.set_error_channels(websocketpp::log::elevel::all); 
        endpoint.set_access_channels(websocketpp::log::alevel::all ^
            websocketpp::log::alevel::frame_payload);

        /* Initialize Asio */
        endpoint.init_asio(); 

        /* Register new handlers */
        endpoint.set_message_handler(&onMessage);
        endpoint.set_open_handler(&onOpenConnection);
        endpoint.set_close_handler(&onCloseConnection);
    }
    catch(websocketpp::exception e)
    {
        cout << "server init error: " << e.what() << endl;
        exit(1);
    }
}


void GameServer::run()
{
    try
    {
        /* Listen on port */
        endpoint.listen(SERVER_PORT);

        /* Queues a connection accept operation */ 
        endpoint.start_accept();

        /* Start the Asio io_service run loop */
        endpoint.run(); 
    }
    catch(websocketpp::exception e)
    {
        cout << "listen error: " << e.what() << endl;
        exit(1);
    }
}


void GameServer::stop()
{
    /* stop accepting new connections */
    try
    {
        endpoint.stop_listening();
    }
    catch(websocketpp::exception e)
    {
        cout << "stop_listening exception: " << e.what() << endl;
    }

    /* close all existing connections */
    for(unsigned int i = 0; i < connections.size(); i++)
    {
        try
        {
            endpoint.pause_reading(connections[i]);
            endpoint.close(connections[i], websocketpp::close::status::going_away, 
                "Server shutdown");
        }
        catch(websocketpp::exception e)
        {
            cout << "close connection exception: " << e.what() << endl;
        }
    }
}


void GameServer::onMessage(Connection connection, Message msg)
{
    cout << "Message received: \"" << msg->get_payload() << "\"" << endl;

    if (msg->get_payload() == "stop-server")
    {
        stop();
        return;
    }

    if (msg->get_payload() == "nb-connections")
    {
        cout << "Number of connections: " << connections.size() << endl;
        return;
    }

    treatMessage(msg->get_payload());
}


void GameServer::onOpenConnection(Connection connection)
{
    /* add newly opened connection to connections list */
    connections.push_back(connection);
}


void GameServer::onCloseConnection(Connection connection)
{
    bool found = false;

    /* naive removing connection */
    for (unsigned int i = 0; i < connections.size() && !found; i++)
    {
        if (connections[i].lock().get() == connection.lock().get())
        {
            connections.erase(connections.begin() + i);
            cout << "Removing connection " << connection.lock().get() 
                << ": connection closed" << endl;
            found = true;
        }
    }
}


/* DEBUGGING */

int GameServer::nbGames()
{
    return games.size();
}