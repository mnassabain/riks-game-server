#include "../header/GameServer.h"

/* debug */
#define MAX_PLAYERS 6
#define MAX_GAMES   10



vector<Game> GameServer::games;
ServerEndpoint GameServer::endpoint;
map<string, Connection> GameServer::clients;
vector<Connection> GameServer::unregisteredConnections;


void GameServer::listen()
{
    init();
    run();
}


string GameServer::treatMessage(string message, Connection connection)
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

        errorResponse(response, CODE_ERROR, "Invalid message format");
        return response.dump();
    }


    if (jmessage.size() <= 0 || !jmessage.count("type"))
    {
        errorResponse(response, CODE_ERROR, "Invalid message format");
        return response.dump();
    }

    if (!jmessage["type"].is_number())
    {
        errorResponse(response, CODE_ERROR, "Invalid message type");
        return response.dump();
    }

    MessageCode code = jmessage["type"];
    switch(code)
    {
        case CODE_SIGN_UP:

            if (!jmessage.count("data"))
            {
                errorResponse(response, CODE_SIGN_UP, 
                    "Invalid message format; insufficient parameters");

                break;
            }

            if (!jmessage["data"].is_object())
            {
                errorResponse(response, CODE_SIGN_UP, 
                    "Invalid message data types");

                break;
            }

            if (!jmessage["data"].count("userID") 
                || !jmessage["data"].count("userPassword"))
            {
                errorResponse(response, CODE_SIGN_UP, 
                    "Invalid message format; insufficient parameters");

                break;
            }

            if (!jmessage["data"]["userID"].is_string() ||
                !jmessage["data"]["userPassword"].is_string())
            {
                errorResponse(response, CODE_SIGN_UP, 
                    "Invalid message data types");

                break;
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
                errorResponse(response, CODE_CONNECT, 
                    "Invalid message format; insufficient parameters");

                break;
            }

            if (!jmessage["data"].is_object())
            {
                errorResponse(response, CODE_CONNECT,
                    "Invalid message data types");

                break;
            }

            if (!jmessage["data"].count("userID") 
                || !jmessage["data"].count("userPassword"))
            {
                errorResponse(response, CODE_CONNECT, 
                    "Invalid message format; insufficient parameters");

                break;
            }

            if (!jmessage["data"]["userID"].is_string() ||
                !jmessage["data"]["userPassword"].is_string())
            {
                errorResponse(response, CODE_CONNECT,
                    "Invalid message data types");

                break;   
            }
            
            cout << "Connection attempt by user (id = " 
                << jmessage["data"]["userID"] << ", password = " 
                << jmessage["data"]["userPassword"] << ")" << endl;
                
            /* ... */

            /* move from unregistered connections into registered clients */
            clients.emplace(jmessage["data"]["userID"], connection);

            /* remove from unregistered connections */
            {
                bool found = false;
                for (unsigned int i = 0; 
                    i < unregisteredConnections.size() && !found; 
                    i++)
                {
                    if (unregisteredConnections[i].lock().get() == 
                        connection.lock().get())
                    {
                        found = true;
                        unregisteredConnections.erase(
                            unregisteredConnections.begin() + i);
                    }
                }
            }
            
            response["type"] = CODE_CONNECT;
            response["data"]["error"] = false;
            response["data"]["response"] = "Success";
            break;

        case CODE_DISCONNECT:
            if (!jmessage.count("data"))
            {
                errorResponse(response, CODE_DISCONNECT, 
                    "Invalid message; insufficient parameters");

                break;
            }

            if (!jmessage["data"].is_object())
            {
                errorResponse(response, CODE_DISCONNECT, 
                    "Invalid message data types");

                break;
            }

            if (!jmessage["data"].count("userID"))
            {
                errorResponse(response, CODE_DISCONNECT, 
                    "Invalid message format; insufficient parameters");
                    
                break;
            }

            if (!jmessage["data"]["userID"].is_string())
            {
                errorResponse(response, CODE_DISCONNECT, 
                    "Invalid message data types");
                
                break;
            }

            cout << "User disconnected (id = " << jmessage["data"]["userID"] 
                << ")" << endl;


            response["type"] = CODE_DISCONNECT;
            response["data"]["error"] = false;
            response["data"]["response"] = "Success";
            break;

        case CODE_ERROR:

            /* ? */
            errorResponse(response, CODE_ERROR, "Error");

            break;

        case CODE_CREATE_LOBBY:

            if (!jmessage.count("data"))
            {
                errorResponse(response, CODE_CREATE_LOBBY,
                    "Invalid message; insufficient parameters");

                break;
            }

            if (!jmessage["data"].is_object())
            {
                errorResponse(response, CODE_CREATE_LOBBY,
                    "Invalid message data types");

                break;
            }

            if (!jmessage["data"].count("userID") ||
                !jmessage["data"].count("lobbyName") ||
                !jmessage["data"].count("lobbyPassword") ||
                !jmessage["data"].count("maxPlayers") ||
                !jmessage["data"].count("mapName"))
            {
                errorResponse(response, CODE_CREATE_LOBBY,
                    "Invalid message; insufficient parameters");

                break;
            }

            if (!jmessage["data"]["userID"].is_string() ||
                !jmessage["data"]["lobbyName"].is_string() ||
                !jmessage["data"]["lobbyPassword"].is_string() ||
                !jmessage["data"]["maxPlayers"].is_number() ||
                !jmessage["data"]["mapName"].is_string())
            {
                errorResponse(response, CODE_CREATE_LOBBY,
                    "Invalid message data types");

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
                errorResponse(response, CODE_JOIN_LOBBY,
                    "Invalid message; insufficient parameters");

                break;
            }

            if (!jmessage["data"].is_object())
            {
                errorResponse(response, CODE_JOIN_LOBBY,
                    "Invalid message data types");

                break;
            }    
            
            if (!jmessage["data"].count("playerID") ||
                !jmessage["data"].count("lobbyID") ||
                !jmessage["data"].count("lobbyPassword")
                )
            {
                errorResponse(response, CODE_JOIN_LOBBY,
                    "Invalid message; insufficient parameters");

                break;
            }

            if (!jmessage["data"]["playerID"].is_string() ||
                !jmessage["data"]["lobbyID"].is_number() ||
                !jmessage["data"]["lobbyPassword"].is_string())
            {
                errorResponse(response, CODE_JOIN_LOBBY,
                    "Invalid message data types");

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
                            errorResponse(response, CODE_JOIN_LOBBY,
                                "Lobby is full");
                        }
                        else
                        {
                            games[i].addPlayer(jmessage["data"]["playerID"]);

                            response["type"] = CODE_JOIN_LOBBY;
                            response["data"]["error"] = false;
                            response["data"]["response"] = "Success";
                        }
                    }
                }

                if (!found)
                {
                    errorResponse(response, CODE_JOIN_LOBBY,
                    "Lobby not found");
                }
            }
            break;

        
        default:
            cout << "Unhandled message code" << endl;

            errorResponse(response, CODE_JOIN_LOBBY, "Unrecognized message");
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
    map<string, Connection>::iterator i;
    for(i = clients.begin(); i != clients.end(); i++)
    {
        try
        {
            endpoint.pause_reading(i->second);
            endpoint.close(i->second, 
                websocketpp::close::status::going_away, "Server shutdown");
        }
        catch(websocketpp::exception e)
        {
            cout << "close connection exception: " << e.what() << endl;
        }
    }

    for (unsigned int j = 0; j < unregisteredConnections.size(); j++)
    {
        try
        {
            endpoint.pause_reading(unregisteredConnections[j]);
            endpoint.close(unregisteredConnections[j], 
                websocketpp::close::status::going_away, "Server shutdown");
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
        cout << "Number of connections: " << nbConnections()<< endl;
        return;
    }

    if (msg->get_payload() == "nb-unregistered-connections")
    {
        cout << "Number of unregistered connections: " 
            << nbUnregisteredConnections() << endl;
        return;
    }

    if (msg->get_payload() == "nb-clients")
    {
        cout << "Number of clients: " << nbClients() << endl;
        return;
    }

    string response = treatMessage(msg->get_payload(), connection);

    endpoint.send(connection, response, websocketpp::frame::opcode::text);
}


void GameServer::onOpenConnection(Connection connection)
{
    /* add newly opened connection to connections list */
    unregisteredConnections.push_back(connection);
}


void GameServer::onCloseConnection(Connection connection)
{
    bool found = false;

    /* check clients list */
    map<string, Connection>::iterator i;
    for (i = clients.begin(); i != clients.end(); i++)
    {
        if (i->second.lock().get() == connection.lock().get())
        {
            found = true;
            clients.erase(i);
        }
    }


    if (found)
        return;

    /* check unregistered connections */
    for (unsigned int j = 0; j < unregisteredConnections.size() && !found; j++)
    {
        if (unregisteredConnections[j].lock().get() == connection.lock().get())
        {
            unregisteredConnections.erase(unregisteredConnections.begin() + j);
            cout << "Removing connection " << connection.lock().get() 
                << ": connection closed" << endl;
            found = true;
        }
    }
}


void GameServer::errorResponse(json& response, MessageCode code, string message)
{
    response["type"] = code;
    response["data"]["error"] = true;
    response["data"]["response"] = message;
}



/* DEBUGGING */

int GameServer::nbGames()
{
    return games.size();
}


int GameServer::nbConnections()
{
    return nbClients() + nbUnregisteredConnections();
}

int GameServer::nbClients()
{
    return clients.size();
}

int GameServer::nbUnregisteredConnections()
{
    return unregisteredConnections.size();
}