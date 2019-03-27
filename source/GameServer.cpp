#include "../header/GameServer.h"

/* debug */
#define MAX_PLAYERS 6
#define MAX_GAMES   10
#define DB_NAME "DB/riksserverdb.db"


const int GameServer::SERVER_HUB = -1;
map<int, Game> GameServer::games;
ServerEndpoint GameServer::endpoint;
map<void*, Client> GameServer::clients;
vector<Connection> GameServer::unregisteredConnections;


void GameServer::listen()
{
    /* Initialise server */
    init();

    /* Start accepting connections & messages */
    run();
}


string GameServer::treatMessage(string message, Connection connection)
{
    /* structure in which the message will be parsed into json */
    json jmessage;

    /* structure which will contain the response */
    json response;

    /* we parse the message */
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

    /* if the message is not json we return an error */
    if (jmessage.size() <= 0 || !jmessage.count("type"))
    {
        errorResponse(response, CODE_ERROR, "Invalid message format");
        return response.dump();
    }

    /* we check if the message has a valid type */
    if (!jmessage["type"].is_number())
    {
        errorResponse(response, CODE_ERROR, "Invalid message type");
        return response.dump();
    }

    /* we extract the message type (message code) */
    MessageCode code = jmessage["type"];
    switch(code)
    {
        case CODE_SIGN_UP:

            /* we check if the user is not already connected */
            {
                map<void*, Client>::iterator client;
                client = clients.find(connection.lock().get());

                if (client != clients.end())
                {
                    errorResponse(response, CODE_SIGN_UP, 
                        "SIGN UP: Already logged in");
                    break;
                }
            }

            /* we check if the message has a data field */
            if (!jmessage.count("data"))
            {
                errorResponse(response, CODE_SIGN_UP,
                    "Invalid message format; insufficient parameters");

                break;
            }

            /* we check if the data field is an object */
            if (!jmessage["data"].is_object())
            {
                errorResponse(response, CODE_SIGN_UP,
                    "Invalid message data types");

                break;
            }

            /* we check if the user provided an id and password */
            if (!jmessage["data"].count("userID")
                || !jmessage["data"].count("userPassword"))
            {
                errorResponse(response, CODE_SIGN_UP,
                    "Invalid message format; insufficient parameters");

                break;
            }

            /* we check the formats of the id and password */
            if (!jmessage["data"]["userID"].is_string() ||
                !jmessage["data"]["userPassword"].is_string())
            {
                errorResponse(response, CODE_SIGN_UP,
                    "Invalid message data types");

                break;
            }

            /* logging */
            cout << "Creating new user (id = " << jmessage["data"]["userID"]
                << ", password = " << jmessage["data"]["userPassword"] << ")"
                << endl;

	    {//====insertion into db====

	    sqlite3* db;
	    int returnCode = 0;
	    sqlite3_stmt* stmt = NULL;

	    //Connection
	    returnCode = sqlite3_open_v2(DB_NAME,&db,SQLITE_OPEN_READWRITE,NULL);

	    if(returnCode)//Connection error
	    {
		cout<<"SIGN_UP : DB connection error : "<<sqlite3_errmsg(db)<<endl;
		sqlite3_close(db);
		response["type"]= CODE_SIGN_UP;
		response["data"]["error"]=true;
		response["data"]["response"]="SIGN_UP Data Base connection error";
	    }
	    else//DB is connected
	    {
		returnCode = sqlite3_prepare(db,"Insert into users values(null,?,?)",-1,&stmt,0);
		if(returnCode)//Error when creating request
		{
			cout<<"SIGN_UP : Insert preparing error"<<endl;
			sqlite3_close(db);
			response["type"]= CODE_SIGN_UP;
			response["data"]["error"]=true;
			response["data"]["response"]="SIGN_UP Error during Insertion preparing";

		}
		else//No preparing error
		{
			string uid = jmessage["data"]["userID"].get<string>();
			string pwd = jmessage["data"]["userPassword"].get<string>();
			if(sqlite3_bind_text(stmt,1,uid.c_str(),uid.length(),SQLITE_STATIC) || sqlite3_bind_text(stmt,2,pwd.c_str(),pwd.length(),SQLITE_STATIC) ) //Error while binding parameters
			{
			  cout<<"SIGN_UP : Binding parmeters error"<<endl;
			  sqlite3_close(db);
			  response["type"]= CODE_SIGN_UP;
			  response["data"]["error"]=true;
			  response["data"]["response"]="SIGN_UP Error during binding";
			}
			else//No binding error
			{
			  if(sqlite3_step(stmt) != SQLITE_DONE)
			  {
			    cout<<"SIGN_UP : Insert step (executing) error"<<endl;
			    sqlite3_close(db);
		 	    response["type"]= CODE_SIGN_UP;
			    response["data"]["error"]=true;
			    response["data"]["response"]="SIGN_UP Error during Insertion execution";
			  }
			  else// SIGN_UP Success
			  {

			    cout<<"SIGN_UP : Success inserting new user"<<endl;
			    sqlite3_close(db);
			    response["type"]= CODE_SIGN_UP;
			    response["data"]["error"]=false;
			    response["data"]["response"]="Success";
			  }
			}
		}
	    }


            sqlite3_finalize(stmt);
	    }
            break;

        case CODE_CONNECT:

            /* we check if the user is already connected */
            {
                map<void*, Client>::iterator client;
                client = clients.find(connection.lock().get());

                if (client != clients.end())
                {
                    errorResponse(response, CODE_CONNECT, 
                        "CONNECT: Already logged in");
                    break;
                }
            }

            /* we check if the message contains a data field */
            if (!jmessage.count("data"))
            {
                errorResponse(response, CODE_CONNECT,
                    "Invalid message format; insufficient parameters");

                break;
            }

            /* we check if the data field is an object */
            if (!jmessage["data"].is_object())
            {
                errorResponse(response, CODE_CONNECT,
                    "Invalid message data types");

                break;
            }

            /* we check if the message contains an id and a password */
            if (!jmessage["data"].count("userID")
                || !jmessage["data"].count("userPassword"))
            {
                errorResponse(response, CODE_CONNECT,
                    "Invalid message format; insufficient parameters");

                break;
            }

            /* we check the type of the id and the password */
            if (!jmessage["data"]["userID"].is_string() ||
                !jmessage["data"]["userPassword"].is_string())
            {
                errorResponse(response, CODE_CONNECT,
                    "Invalid message data types");

                break;
            }

            /* logging */
            cout << "Connection attempt by user (id = "
                << jmessage["data"]["userID"] << ", password = "
                << jmessage["data"]["userPassword"] << ")" << endl;

	    {//====select user in db====
	      sqlite3* db;
	      int returnCode = 0;
	      sqlite3_stmt* stmt = NULL;

	      //Connection
	      returnCode = sqlite3_open_v2(DB_NAME,&db,SQLITE_OPEN_READWRITE,NULL);

	      if(returnCode)//Connection error
	      {
		cout<<"CONNECT : DB connection error : "<<sqlite3_errmsg(db)<<endl;
		sqlite3_close(db);
		response["type"]= CODE_CONNECT;
		response["data"]["error"]=true;
		response["data"]["response"]="CONNECT Data Base connection error";
	      }
	      else //No connection error
	      {
		if(sqlite3_prepare_v2(db,"Select * from users where login=?",-1,&stmt,NULL)) //Error when creating request
		{
		  cout<<"CONNECT : Select preparing error"<<endl;
		  sqlite3_close(db);
		  response["type"]= CODE_CONNECT;
		  response["data"]["error"]=true;
		  response["data"]["response"]="CONNECT Data Base Select preparing error";

		}
		else //No preparing error
		{
		  string uid = jmessage["data"]["userID"].get<string>();
	   	  const char * pwd = jmessage["data"]["userPassword"].get<string>().c_str();
		  if(sqlite3_bind_text(stmt,1,uid.c_str(),uid.length(),SQLITE_STATIC)) //Error while binding parameters
		  {
		    cout<<"CONNECT : Binding parmeters error"<<endl;
		    sqlite3_close(db);
	  	    response["type"]= CODE_CONNECT;
		    response["data"]["error"]=true;
		    response["data"]["response"]="CONNECT Error during binding";
		  }
		  else//No binding error
		  {
		    returnCode = sqlite3_step(stmt);
		    if(returnCode == SQLITE_OK || returnCode == SQLITE_ROW)//Found
		    {
		      unsigned char * realPwd = (unsigned char*)sqlite3_column_text(stmt,2);//Getting password
		      if(strcmp(pwd,(char *)realPwd))//True if passwords are different
		      {
			cout<<"CONNECT : Bad password"<<endl;
		    	sqlite3_close(db);
	  	    	response["type"]= CODE_CONNECT;
		    	response["data"]["error"]=true;
		    	response["data"]["response"]="CONNECT Bad password";
		      }
		      else //CONNECT Success
		      {
            		cout<<"CONNECT : User successfully connected"<<endl;
			sqlite3_close(db);
			response["type"] = CODE_CONNECT;
            		response["data"]["error"] = false;
            		response["data"]["response"] = "Success";
		      }
		    }
		    else //User not found
		    {
		      cout<<"CONNECT : Bad Login (user not found)"<<endl;
		      sqlite3_close(db);
	  	      response["type"]= CODE_CONNECT;
		      response["data"]["error"]=true;
		      response["data"]["response"]="CONNECT Bad Login (user not found)";
		    }
		  }

		}
		
	      }
	     sqlite3_finalize(stmt);
	    }

            /* move from unregistered connections into registered clients */
            clients.emplace(connection.lock().get(), 
                Client(jmessage["data"]["userID"], connection));

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

            break;

        case CODE_DISCONNECT:

            /* check if user is connected */
            {
                map<void*, Client>::iterator it;
                it = clients.find(connection.lock().get());

                /* if we can't find the user we send an error */
                if (it == clients.end())
                {
                    errorResponse(response, CODE_DISCONNECT,
                        "DISCONNECT: User not connected");
                    break;
                }

                /* disconnect user */
                try
                {
                    endpoint.pause_reading(connection);
                    endpoint.close(connection, 
                        websocketpp::close::status::normal, 
                        "Successfully disconnected");
                }
                catch(exception e)
                {
                    cout << "Caught websocket exception: " 
                        << e.what() << endl;
                }

                /* logging */
                cout << "User disconnected (id = " << it->second.getName() 
                    << ")" << endl;

                /* remove disconnected user from his game if he is in one */
                int gameID = it->second.getGameID();
                if (gameID != SERVER_HUB)
                {
                    map<int, Game>::iterator game;
                    game = games.find(gameID);

                    if (game != games.end())
                    {
                        game->second.removePlayer(it->second.getName());
                    }
                }

                /* remove disconnected user from clients map */
                clients.erase(it);
            }

            /* construct response */
            response["type"] = CODE_DISCONNECT;
            response["data"]["error"] = false;
            response["data"]["response"] = "Success";
            break;

        case CODE_ERROR:

            /* ? */
            errorResponse(response, CODE_ERROR, "Error");

            break;

        case CODE_CREATE_LOBBY:

            /* we check if the message has a data field */
            if (!jmessage.count("data"))
            {
                errorResponse(response, CODE_CREATE_LOBBY,
                    "Invalid message; insufficient parameters");

                break;
            }

            /* we check the data field type */
            if (!jmessage["data"].is_object())
            {
                errorResponse(response, CODE_CREATE_LOBBY,
                    "Invalid message data types");

                break;
            }

            /* we check if the data field contains the necessary info */
            if (!jmessage["data"].count("lobbyName") ||
                !jmessage["data"].count("lobbyPassword") ||
                !jmessage["data"].count("maxPlayers") ||
                !jmessage["data"].count("mapName"))
            {
                errorResponse(response, CODE_CREATE_LOBBY,
                    "Invalid message; insufficient parameters");

                break;
            }

            /* and we check if the info is in the correct type */
            if (!jmessage["data"]["lobbyName"].is_string() ||
                !jmessage["data"]["lobbyPassword"].is_string() ||
                !jmessage["data"]["maxPlayers"].is_number() ||
                !jmessage["data"]["mapName"].is_string())
            {
                errorResponse(response, CODE_CREATE_LOBBY,
                    "Invalid message data types");

                break;
            }

            /* check if user is connected */
            {
                map<void*, Client>::iterator it;
                it = clients.find(connection.lock().get());

                /* if we can't find the user we send an error */
                if (it == clients.end())
                {
                    errorResponse(response, CODE_CREATE_LOBBY,
                        "CREATE LOBBY: User not connected");
                    break;
                }

                /* we check if the user is not already in a game */
                if (it->second.getGameID() != SERVER_HUB)
                {
                    errorResponse(response, CODE_CREATE_LOBBY,
                        "CREATE LOBBY: User already in lobby/game");
                    break;
                }

                /* logging */
                cout << "User " << it->second.getName()
                    << ": attempt to create lobby "
                    << jmessage["data"]["lobbyName"]
                    << "with password" << jmessage["data"]["lobbyPassword"]
                    << "of max players " << jmessage["data"]["maxPlayers"]
                    << "and on map " << jmessage["data"]["mapName"] << endl;

            
                /* we create a new game */
                int newGameId = createGame(jmessage["data"]["mapName"],
                    it->second.getName(), jmessage["data"]["maxPlayers"]);
                it->second.setGameID(newGameId);

                /* logging */
                cout << "Lobby created" << endl;

                /* construct the response */
                response["type"] = CODE_CREATE_LOBBY;
                response["data"]["error"] = false;
                response["data"]["response"] = "Success";
                response["data"]["lobbyID"] = newGameId;
            }
            break;

        case CODE_LOBBY_LIST:

            /* logging */
            cout << "Received lobby list demand" << endl;

            /* check if user is connected */
            {
                map<void*, Client>::iterator it;
                it = clients.find(connection.lock().get());

                /* if we can't find the user we send an error */
                if (it == clients.end())
                {
                    errorResponse(response, CODE_LOBBY_LIST,
                        "LOBBY LIST: User not connected");
                    break;
                }

                /* check if the user is in a game */
                if (it->second.getGameID() != SERVER_HUB)
                {
                    errorResponse(response, CODE_LOBBY_LIST,
                        "LOBBY LIST: User already in lobby/game");
                    break;
                }
            }

            /* construct the first part of the response */
            response["type"] = CODE_LOBBY_LIST;
            response["data"]["error"] = false;
            response["data"]["response"] = "Success";

            /* we get the list of running games and we insert them into the 
             * response 
             */
            {
                int nb = 0;
                map<int, Game>::iterator it;
                for (it = games.begin(); nb < MAX_GAMES && it != games.end();
                    it++)
                {
                    if (!it->second.isRunning())
                    {
                        response["data"]["gameList"].push_back(
                            it->second.toJSON());
                        nb++;
                    }
                }
            }
            break;

        case CODE_JOIN_LOBBY:

            /* we check if the message has a data field */
            if (!jmessage.count("data"))
            {
                errorResponse(response, CODE_JOIN_LOBBY,
                    "Invalid message; insufficient parameters");

                break;
            }

            /* we check the data field type */
            if (!jmessage["data"].is_object())
            {
                errorResponse(response, CODE_JOIN_LOBBY,
                    "Invalid message data types");

                break;
            }

            /* we check if the data field contains the necessary info */
            if (!jmessage["data"].count("lobbyID") ||
                !jmessage["data"].count("lobbyPassword")
                )
            {
                errorResponse(response, CODE_JOIN_LOBBY,
                    "Invalid message; insufficient parameters");

                break;
            }

            /* we check if the info has the correct type */
            if (!jmessage["data"]["lobbyID"].is_number() ||
                !jmessage["data"]["lobbyPassword"].is_string())
            {
                errorResponse(response, CODE_JOIN_LOBBY,
                    "Invalid message data types");

                break;
            }

            /* check if user is connected */
            {
                map<void*, Client>::iterator it;
                it = clients.find(connection.lock().get());

                /* if we can't find the user we send an error */
                if (it == clients.end())
                {
                    errorResponse(response, CODE_JOIN_LOBBY,
                        "JOIN LOBBY: User not connected");
                    break;
                }

                /* check if the user is in a game */
                if (it->second.getGameID() != SERVER_HUB)
                {
                    errorResponse(response, CODE_JOIN_LOBBY,
                        "JOIN LOBBY: User already in lobby/game");
                    break;
                }

                /* logging */
                cout << "User " << it->second.getName()
                << " tried to join lobby with id: "
                << jmessage["data"]["lobbyID"]
                << "and password: \"" << jmessage["data"]["lobbyPassword"]
                << "\"" << endl;

                /* we look for the lobby */
                map<int, Game>::iterator game;
                game = games.find(jmessage["data"]["lobbyID"]);

                /* if the lobby is not found send error response */
                if (game == games.end())
                {
                    errorResponse(response, CODE_JOIN_LOBBY, "Lobby not found");
                    break;
                }

                /* check if the lobby is full */
                if (game->second.isFull())
                {
                    errorResponse(response, CODE_JOIN_LOBBY, "Lobby is full");
                    break;
                }

                /* check if the password is correct */
                if (game->second.getPassword() 
                    != jmessage["data"]["lobbyPassword"])
                {
                    errorResponse(response, CODE_JOIN_LOBBY, 
                        "Incorrect password");
                    break;
                }

                /* if all tests pass, we can join the game */
                game->second.addPlayer(it->second.getName());
                it->second.setGameID(jmessage["data"]["lobbyID"]);

                /* construct response */
                response["type"] = CODE_JOIN_LOBBY;
                response["data"]["error"] = false;
                response["data"]["response"] = "Success";

                /* give all existing players new lobby state */
                json update;
                update["type"] = CODE_LOBBY_STATE;
                update["data"]["error"] = false;
                update["data"]["response"] = "Success";
                update["data"]["gameData"] = game->second.toJSON();

                vector<Player> players = game->second.getPlayers();
                for (unsigned int i = 0; i < players.size(); i++)
                {
                    map<void*, Client>::iterator player;

                    player = clients.find(connection.lock().get());
                    Connection c = player->second.getConnection();

                    endpoint.send(c, update.dump(), 
                        websocketpp::frame::opcode::text);
                }
            }
            break;

        case CODE_LEAVE_GAME:

            /* check if user is connected */
            {
                map<void*, Client>::iterator it;
                it = clients.find(connection.lock().get());

                if (it == clients.end())
                {
                    errorResponse(response, CODE_LEAVE_GAME, 
                        "User not connected");
                    break;
                }

                /* check if client is in the lobby/game */
                map<int, Game>::iterator game;
                game = games.find(it->second.getGameID());

                /* if we can't find the game we send an error */
                if (game == games.end())
                {
                    errorResponse(response, CODE_LEAVE_GAME, 
                        "User not in lobby");
                    break;
                }
                
                /* if we find the game we remove the player from the game */
                game->second.removePlayer(it->second.getName());
                it->second.setGameID(SERVER_HUB);

                /* and if the room is empty we delete it */
                if (game->second.getNbPlayers() == 0)
                {
                    games.erase(game);
                }
            }

            /* construct the response */
            response["type"] = CODE_LEAVE_GAME;
            response["data"]["error"] = false;
            response["data"]["response"] = "Success";

            break;

        case CODE_LOBBY_STATE:

            /* check if user is connected */
            {
                map<void*, Client>::iterator it;
                it = clients.find(connection.lock().get());

                if (it == clients.end())
                {
                    errorResponse(response, CODE_LOBBY_STATE,
                        "User not connected");
                    break;
                }

                map<int, Game>::iterator game;
                game = games.find(it->second.getGameID());
                
                /* if we don't find the lobby we send an error */
                if (game == games.end())
                {
                    errorResponse(response, CODE_LOBBY_STATE, 
                        "Lobby not found");
                    break;
                }

                /* insert the game data into the response */
                response["data"]["gameData"] = game->second.toJSON();
            }

            /* construct the response */
            response["type"] = CODE_LOBBY_STATE;
            response["data"]["error"] = false;
            response["data"]["response"] = "Success";

            break;


        default:
            /* if we received an unhandled message code we tell the client */
            /* logging */
            cout << "Unhandled message code" << endl;

            /* construct the response */
            errorResponse(response, CODE_UNHANDLED, "Unrecognized message");
    }

    /* turn json object into string */
    return response.dump();
}


int GameServer::createGame(string mapName, string host, int nbPlayers)
{
    /* create a new game */
    Game newGame(mapName, host, MAX_PLAYERS);

    /* copy the new object into the games list */
    int newGameID = newGame.getId();
    games.emplace(newGameID, newGame);

    return newGameID;
}


int GameServer::destroyGame(int id)
{
    map<int, Game>::iterator it;
    it = games.find(id);

    if (it == games.end())
    {
        cout << "Game " << id << " not found" << endl;
        return -1;
    }

    games.erase(it);
    return 0;
}


/* PRIVATE METHODS */

void GameServer::init()
{
    try
    {
        /* reuse same port */
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
    map<void*, Client>::iterator i;
    for(i = clients.begin(); i != clients.end(); i++)
    {
        try
        {
            endpoint.pause_reading(i->second.getConnection());
            endpoint.close(i->second.getConnection(),
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
    /* logging */
    cout << "Message received: \"" << msg->get_payload() << "\"" << endl;

    /* logging messages */
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

    if (msg->get_payload() == "nb-games")
    {
        cout << "Number of games: " << nbGames() << endl;
        return;
    }

    /* treat message */
    string response = treatMessage(msg->get_payload(), connection);

    /* send the response to the client */
    try
    {
        endpoint.send(connection, response, websocketpp::frame::opcode::text);
    }
    catch(websocketpp::exception e)
    {
        cout << "Caught exception when sending message: " << e.what() << endl;
    }
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
    map<void*, Client>::iterator i;
    i = clients.find(connection.lock().get());

    if (i != clients.end())
    {
        clients.erase(i);
        found = true;
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
    return clients.size() + nbUnregisteredConnections();
}

int GameServer::nbClients()
{
    return clients.size();
}

int GameServer::nbUnregisteredConnections()
{
    return unregisteredConnections.size();
}
