#include "../header/GameServer.h"

/* debug */
#define MAX_PLAYERS 6
#define MAX_GAMES   10
#define DB_NAME "DB/riksserverdb.db"



map<int, Game> GameServer::games;
ServerEndpoint GameServer::endpoint;
map<void*, Client> GameServer::clients;
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
            // if (!jmessage.count("data"))
            // {
            //     errorResponse(response, CODE_DISCONNECT,
            //         "Invalid message; insufficient parameters");

            //     break;
            // }

            // if (!jmessage["data"].is_object())
            // {
            //     errorResponse(response, CODE_DISCONNECT,
            //         "Invalid message data types");

            //     break;
            // }

            // if (!jmessage["data"].count("userID"))
            // {
            //     errorResponse(response, CODE_DISCONNECT,
            //         "Invalid message format; insufficient parameters");

            //     break;
            // }

            // if (!jmessage["data"]["userID"].is_string())
            // {
            //     errorResponse(response, CODE_DISCONNECT,
            //         "Invalid message data types");

            //     break;
            // }

            /* check if user is connected & owns connection */
            {
                map<void*, Client>::iterator it;
                it = clients.find(connection.lock().get());

                /* if we can't find the user we send an error */
                if (it == clients.end())
                {
                    errorResponse(response, CODE_DISCONNECT,
                        "User not connected");
                    break;
                }

                /* check if user owns the connection */
                // if (it->second.getConnection().lock().get() 
                //     != connection.lock().get())
                // {
                //     errorResponse(response, CODE_DISCONNECT,
                //         "Action not permitted");
                //     break;
                // }

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

                cout << "User disconnected (id = " << it->second.getName() 
                    << ")" << endl;

                /* remove disconnected user from clients map */
                clients.erase(it);
            }


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

            if (!jmessage["data"].count("lobbyName") ||
                !jmessage["data"].count("lobbyPassword") ||
                !jmessage["data"].count("maxPlayers") ||
                !jmessage["data"].count("mapName"))
            {
                errorResponse(response, CODE_CREATE_LOBBY,
                    "Invalid message; insufficient parameters");

                break;
            }

            if (!jmessage["data"]["lobbyName"].is_string() ||
                !jmessage["data"]["lobbyPassword"].is_string() ||
                !jmessage["data"]["maxPlayers"].is_number() ||
                !jmessage["data"]["mapName"].is_string())
            {
                errorResponse(response, CODE_CREATE_LOBBY,
                    "Invalid message data types");

                break;
            }

            /* check if user is connected & owns connection */
            {
                map<void*, Client>::iterator it;
                it = clients.find(connection.lock().get());

                /* if we can't find the user we send an error */
                if (it == clients.end())
                {
                    errorResponse(response, CODE_CREATE_LOBBY,
                        "User not connected");
                    break;
                }

                /* check if user owns the connection */
                // if (it->second.getConnection().lock().get() 
                    // != connection.lock().get())
                // {
                    // errorResponse(response, CODE_CREATE_LOBBY,
                        // "Action not permitted");
                    // break;
                // }
            

                cout << "User " << it->second.getName()
                    << ": attempt to create lobby "
                    << jmessage["data"]["lobbyName"]
                    << "with password" << jmessage["data"]["lobbyPassword"]
                    << "of max players " << jmessage["data"]["maxPlayers"]
                    << "and on map " << jmessage["data"]["mapName"] << endl;

            
                int newGameId = createGame(jmessage["data"]["mapName"],
                    it->second.getName(), jmessage["data"]["maxPlayers"]);
                it->second.setGameID(newGameId);

                cout << "Lobby created" << endl;

                response["type"] = CODE_CREATE_LOBBY;
                response["data"]["error"] = false;
                response["data"]["response"] = "Success";
                response["data"]["lobbyID"] = newGameId;
            }
            break;

        case CODE_LOBBY_LIST:

            cout << "Received lobby list demand" << endl;

            // if (!jmessage.count("data"))
            // {
            //     errorResponse(response, CODE_LOBBY_LIST,
            //         "Invalid message; insufficient parameters");
            //     break;
            // }

            // if (!jmessage["data"].is_object())
            // {
            //     errorResponse(response, CODE_LOBBY_LIST,
            //         "Invalid message data types");
            //     break;
            // }

            // if (!jmessage["data"].count("playerID"))
            // {
            //     errorResponse(response, CODE_LOBBY_LIST,
            //         "Invalid message; insufficient parameters");
            //     break;
            // }

            // if (!jmessage["data"]["playerID"].is_string())
            // {
            //     errorResponse(response, CODE_LOBBY_LIST,
            //         "Invalid message data types");
            //     break;
            // }

            /* check if user is connected & owns connection */
            {
                map<void*, Client>::iterator it;
                it = clients.find(connection.lock().get());

                /* if we can't find the user we send an error */
                if (it == clients.end())
                {
                    errorResponse(response, CODE_LOBBY_LIST,
                        "User not connected");
                    break;
                }

                /* check if user owns the connection */
                // if (it->second.getConnection().lock().get() 
                //     != connection.lock().get())
                // {
                //     errorResponse(response, CODE_LOBBY_LIST,
                //         "Action not permitted");
                //     break;
                // }
            }

            response["type"] = CODE_LOBBY_LIST;
            response["data"]["error"] = false;
            response["data"]["response"] = "Success";

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

            if (!jmessage["data"].count("lobbyID") ||
                !jmessage["data"].count("lobbyPassword")
                )
            {
                errorResponse(response, CODE_JOIN_LOBBY,
                    "Invalid message; insufficient parameters");

                break;
            }

            if (!jmessage["data"]["lobbyID"].is_number() ||
                !jmessage["data"]["lobbyPassword"].is_string())
            {
                errorResponse(response, CODE_JOIN_LOBBY,
                    "Invalid message data types");

                break;
            }

            /* check if user is connected & owns connection */
            {
                map<void*, Client>::iterator it;
                it = clients.find(connection.lock().get());

                /* if we can't find the user we send an error */
                if (it == clients.end())
                {
                    errorResponse(response, CODE_JOIN_LOBBY,
                        "User not connected");
                    break;
                }

                cout << "User " << it->second.getName()
                << " tried to join lobby with id: "
                << jmessage["data"]["lobbyID"]
                << "and password: \"" << jmessage["data"]["lobbyPassword"]
                << "\"" << endl;

                // /* check if user owns the connection */
                // if (it->second.getConnection().lock().get() 
                //     != connection.lock().get())
                // {
                //     errorResponse(response, CODE_JOIN_LOBBY,
                //         "Action not permitted");
                //     break;
                // }


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

            // id, id game
            // if (!jmessage.count("data"))
            // {
            //     errorResponse(response, CODE_LEAVE_GAME,
            //         "Invalid message; insufficient parameters");

            //     break;
            // }

            // if (!jmessage["data"].is_object())
            // {
            //     errorResponse(response, CODE_LEAVE_GAME,
            //         "Invalid message data types");

            //     break;
            // }

            // if (!jmessage["data"].count("lobbyID"))
            // {
            //     errorResponse(response, CODE_LEAVE_GAME,
            //         "Invalid message; insufficient parameters");

            //     break;
            // }

            // if (!jmessage["data"]["lobbyID"].is_number())
            // {
            //     errorResponse(response, CODE_LEAVE_GAME,
            //         "Invalid message data types");

            //     break;
            // }

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

                /* check if the user owns the connections */
                // if (it->second.getConnection().lock().get() 
                //     != connection.lock().get())
                // {
                //     errorResponse(response, CODE_LEAVE_GAME,
                //         "Action not permitted");
                // }
            

            /* check if client is in the lobby/game */

            
                map<int, Game>::iterator game;
                game = games.find(it->second.getGameID());

                /* if we can't find the game we send an error */
                if (game == games.end())
                {
                    errorResponse(response, CODE_LEAVE_GAME, 
                        "Lobby doesn't exist");
                    break;
                }
                
                /* if we find the game we remove the player from the game */
                game->second.removePlayer(it->second.getName());

                /* and if the room is empty we delete it */
                if (game->second.getNbPlayers() == 0)
                {
                    games.erase(game);
                }
            }

            response["type"] = CODE_LEAVE_GAME;
            response["data"]["error"] = false;
            response["data"]["response"] = "Success";

            break;

        case CODE_LOBBY_STATE:

            // if (!jmessage.count("data"))
            // {
            //     errorResponse(response, CODE_LOBBY_STATE,
            //         "Invalid message; insufficient parameters");

            //     break;
            // }

            // if (!jmessage["data"].is_object())
            // {
            //     errorResponse(response, CODE_LOBBY_STATE,
            //         "Invalid message data types");

            //     break;
            // }

            // if (!jmessage["data"].count("gameID"))
            // {
            //     errorResponse(response, CODE_LOBBY_STATE,
            //         "Invalid message; insufficient parameters");

            //     break;
            // }

            // if (!jmessage["data"]["gameID"].is_number())
            // {
            //     errorResponse(response, CODE_LOBBY_STATE,
            //         "Invalid game ID");

            //     break;
            // }

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

                /* check if user owns the connection */
                // if (it->second.getConnection().lock().get() 
                    // != connection.lock().get())
                // {
                    // errorResponse(response, CODE_LOBBY_STATE,
                        // "Action not permitted");
                    // break;
                // }

                map<int, Game>::iterator game;
                game = games.find(it->second.getGameID());
                
                /* if we don't find the lobby we send an error */
                if (game == games.end())
                {
                    errorResponse(response, CODE_LOBBY_STATE, 
                        "Lobby not found");
                    break;
                }

                response["data"]["gameData"] = game->second.toJSON();
            }

            response["type"] = CODE_LOBBY_STATE;
            response["data"]["error"] = false;
            response["data"]["response"] = "Success";

            break;


        default:
            cout << "Unhandled message code" << endl;

            errorResponse(response, CODE_UNHANDLED, "Unrecognized message");
    }

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

    if (msg->get_payload() == "nb-games")
    {
        cout << "Number of games: " << nbGames() << endl;
        return;
    }

    string response = treatMessage(msg->get_payload(), connection);

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
    for (i = clients.begin(); i != clients.end(); i++)
    {
        if (i->second.getConnection().lock().get() == connection.lock().get())
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
