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
    json jmessage;
    json response;

    try
    {
        jmessage = json::parse(message);
    }
    catch(exception e)
    {
        cout << "JSON parse exception: " << e.what() << endl;

        response["type"] = CODE_UNHANDLED;
        response["data"]["error"] = true;
        response["data"]["response"] = "Invalid message";
        return response.dump();
    }


    if (jmessage.size() <= 0 || !jmessage.count("type"))
    {
        response["type"] = CODE_UNHANDLED;
        response["data"]["error"] = true;
        response["data"]["response"] = "Invalid message";
        return response.dump();
    }

    MessageCode code = jmessage["type"];
    switch(code)
    {
        case CODE_SIGN_UP:

            if (!jmessage.count("data"))
            {
                response["type"] = CODE_SIGN_UP;
                response["data"]["error"] = true;
                response["data"]["response"] = 
                    "Invalid message format; insufficient parameters";

                break;
            }
            else
            {
                if (!jmessage["data"].count("userID") 
                    || !jmessage["data"].count("userPassword"))
                {
                    response["type"] = CODE_SIGN_UP;
                    response["data"]["error"] = true;
                    response["data"]["response"] = 
                        "Invalid message format; insufficient parameters";

                    break;
                }
            }
            

            cout << "Creating new user (id = " << jmessage["data"]["userID"]
                << ", password = " << jmessage["data"]["userPassword"] << ")" 
                << endl;

            /* insert into db */

            response["type"] = CODE_SIGN_UP;
            response["data"]["error"] = false;
            response["data"]["response"] = "Success";
            
            break;

        case CODE_CONNECT:

            if (!jmessage.count("data"))
            {
                response["type"] = CODE_CONNECT;
                response["data"]["error"] = true;
                response["data"]["response"] = 
                    "Invalid message format; insufficient parameters";

                break;
            }
            else
            {
                if (!jmessage["data"].count("userID") 
                    || !jmessage["data"].count("userPassword"))
                {
                    response["type"] = CODE_CONNECT;
                    response["data"]["error"] = true;
                    response["data"]["response"] = 
                        "Invalid message format; insufficient parameters";

                    break;
                }
            }

            cout << "Connection attempt by user (id = " 
                << jmessage["data"]["userID"] << ", password = " 
                << jmessage["data"]["userPassword"] << ")" << endl;
                
            /* ... */
            
            response["type"] = CODE_CONNECT;
            response["data"]["error"] = false;
            response["data"]["response"] = "Success";
            break;

        case CODE_DISCONNECT:
            if (!jmessage.count("data"))
            {
                response["type"] = CODE_DISCONNECT;
                response["data"]["error"] = true;
                response["data"]["response"] = 
                    "Invalid message format; insufficient parameters";
                break;
            }

            if (!jmessage["data"].count("userID"))
            {
                response["type"] = CODE_DISCONNECT;
                response["data"]["error"] = true;
                response["data"]["response"] = 
                    "Invalid message format; insufficient parameters";
                break;
            }

            cout << "User disconnected (id = " << jmessage["data"]["userID"] 
                << ")" << endl;


            response["type"] = CODE_DISCONNECT;
            response["data"]["error"] = false;
            response["data"]["response"] = "Success";
            break;

        case CODE_ERROR:
            if (jmessage.size() < 2)
            {
                response.push_back(CODE_ERROR);
                response.push_back("Invalid message");
                break;
            }

            cout << "Error: " << jmessage[1] << endl;
            response.push_back(CODE_ERROR);
            break;

        case CODE_CREATE_LOBBY:

            if (!jmessage.count("data"))
            {
                response["type"] = CODE_CREATE_LOBBY;
                response["data"]["error"] = true;
                response["data"]["response"] = "Invalid message";
                break;
            }

            if (!jmessage["data"].count("userID") ||
                !jmessage["data"].count("lobbyName") ||
                !jmessage["data"].count("lobbyPassword") ||
                !jmessage["data"].count("maxPlayers") ||
                !jmessage["data"].count("mapName"))
            {
                response["type"] = CODE_CREATE_LOBBY;
                response["data"]["error"] = true;
                response["data"]["response"] = "Invalid message";
                break;
            }

            cout << "User " << jmessage["data"]["userID"] 
                << ": attempt to create lobby "
                << jmessage["data"]["lobbyName"]
                << "with password" << jmessage["data"]["lobbyPassword"]
                << "of max players " << jmessage["data"]["maxPlayers"]
                << "and on map " << jmessage["data"]["mapName"] << endl;

            createGame(jmessage["data"]["mapName"], jmessage["data"]["userID"],
                jmessage["data"]["maxPlayers"]);

            cout << "Lobby created" << endl;
            
            response["type"] = CODE_CREATE_LOBBY;
            response["data"]["error"] = false;
            response["data"]["response"] = "Success";
            break;

        case CODE_LOBBY_LIST:

            cout << "Received lobby list demand" << endl;

            response["type"] = CODE_LOBBY_LIST;
            response["data"]["error"] = false;
            response["data"]["response"] = "Success";

            {
                int nb = 0;
                for (unsigned int i = 0; nb < MAX_GAMES && i < games.size();
                    i++)
                {
                    if (!games[i].isRunning())
                    {
                        response["data"]["gameList"].push_back(
                            "game"/*games[i].toJSON()*/);
                        nb++;
                    }
                }
            }
            break;

        case CODE_JOIN_LOBBY:

            if (!jmessage.count("data"))
            {
                response["type"] = CODE_JOIN_LOBBY;
                response["data"]["error"] = true;
                response["data"]["response"] = "Invalid message";

                break;
            }            
            
            if (!jmessage["data"].count("playerID") ||
                !jmessage["data"].count("lobbyID") ||
                !jmessage["data"].count("lobbyPassword")
                )
            {
                response["type"] = CODE_JOIN_LOBBY;
                response["data"]["error"] = true;
                response["data"]["response"] = "Invalid message";

                break;
            }

            cout << "User " << jmessage["data"]["playerID"] 
                << " tried to join lobby with id: "
                << jmessage["data"]["lobbyID"] 
                << "and password: \"" << jmessage["data"]["lobbyPassword"]
                << "\"" << endl;

            {
                bool found = false;
                for (unsigned int i = 0; i < games.size() && !found; i++)
                {
                    if (games[i].getId() == jmessage["data"]["lobbyID"])
                    {
                        found = true;
                        if (false/*games[i].isFull()*/)
                        {
                            response["type"] = CODE_JOIN_LOBBY;
                            response["data"]["error"] = true;
                            response["data"]["response"] = "Lobby is full";
                        }
                        else
                        {
                            games[i].addPlayer(jmessage[1]);
                            response["type"] = CODE_JOIN_LOBBY;
                            response["data"]["error"] = false;
                            response["data"]["response"] = "Success";
                        }
                    }
                }
            }
            break;

        
        default:
            cout << "Unhandled message code" << endl;
            response["type"] = CODE_UNHANDLED;
            response["data"]["error"] = true;
            response["data"]["response"] = "Unrecognized message";
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