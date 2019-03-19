#include "../header/GameServer.h"

/* debug */
#define MAX_PLAYERS 6
#define MAX_GAMES   10



vector<Game> GameServer::games;
ServerEndpoint GameServer::endpoint;
vector<Connection> GameServer::connections;


void GameServer::listen()
{
    init();
    run();
}


string GameServer::treatMessage(string message)
{
    json jmessage = json::parse(message);
    json response;

    MessageCode code = jmessage[0];
    switch(code)
    {
        case CODE_SIGN_UP:
            cout << "Creating new user (id = " << jmessage[1]
                << ", password = " << jmessage[2] << ")" << endl;
            /* insert into db */
            response.push_back(CODE_SIGN_UP);
            break;

        case CODE_CONNECT:
            cout << "Connection attempt by user (id = " << jmessage[1]
                << ", password = " << jmessage[2] << ")" << endl;
            /* ... */
            response.push_back(CODE_CONNECT);
            response.push_back(1234); // token
            break;

        case CODE_DISCONNECT:
            cout << "User disconnected (id = " << jmessage[1] << ")" << endl;
            response.push_back(CODE_DISCONNECT);
            break;

        case CODE_ERROR:
            cout << "Error: " << jmessage[1] << endl;
            response.push_back(CODE_ERROR);
            break;

        case CODE_CREATE_LOBBY:
            cout << "User " << jmessage[1] << ": attempt to create lobby "
                << jmessage[2] << "of max players " << jmessage[3]
                << "and on map " << jmessage[4] << endl;

            createGame(jmessage[4], jmessage[1], jmessage[3]);
            cout << "Lobby created" << endl;
            
            response.push_back(CODE_CREATE_LOBBY);
            break;

        case CODE_LOBBY_LIST:
            cout << "Received lobby list demand" << endl;
            response.push_back(CODE_LOBBY_LIST);

            {
                int nb = 0;
                for (unsigned int i = 0; nb < MAX_GAMES && i < games.size();
                    i++)
                {
                    if (!games[i].isRunning())
                    {
                        response.push_back("game"/*games[i].toString()*/);
                        nb++;
                    }
                }
            }
            break;

        case CODE_JOIN_LOBBY:
            cout << "User " << jmessage[1] << " tried to join lobby with id: "
                << jmessage[2] << "and password: \"" << jmessage[3]
                << "\"" << endl;

            {
                bool found = false;
                for (unsigned int i = 0; i < games.size() && !found; i++)
                {
                    if (games[i].getId() == jmessage[2])
                    {
                        found = true;
                        if (false/*games[i].isFull()*/)
                        {
                            response.push_back(CODE_ERROR);
                            response.push_back("Lobby is full");
                        }
                        else
                        {
                            games[i].addPlayer(jmessage[1]);
                            response.push_back(CODE_JOIN_LOBBY);
                        }
                    }
                }
            }

            break;

        
        default:
            cout << "Unhandled message code" << endl;
            response.push_back(CODE_UNHANDLED);
    }

    return response.dump();
}


void GameServer::createGame(string mapName, string host, int nbPlayers)
{
    /* create a new game */
    Game newGame(mapName, host, MAX_PLAYERS);

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

    string response = treatMessage(msg->get_payload());

    endpoint.send(connection, response, websocketpp::frame::opcode::text);
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