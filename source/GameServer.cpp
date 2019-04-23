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

        errorResponse(response, CODE_ERROR,
            "Invalid message format; cannot be parsed");
        return response.dump();
    }

    /* if the message is not json we return an error */
    if (jmessage.size() <= 0 || !jmessage.count("type"))
    {
        errorResponse(response, CODE_ERROR,
            "Invalid message format; no message type");
        return response.dump();
    }

    /* we check if the message has a valid type */
    if (!jmessage["type"].is_number())
    {
        errorResponse(response, CODE_ERROR,
            "Invalid message format; invalid type");
        return response.dump();
    }

    /* we extract the message type (message code) */
    MessageCode code = jmessage["type"];
    switch(code)
    {
        case CODE_SIGN_UP:

            /* we check if the user is not already connected */
            {
                ClientIterator client;
                client = clients.find(connection.lock().get());

                if (client != clients.end())
                {
                    errorResponse(response, CODE_SIGN_UP,
                        "SIGN_UP: Already logged in");
                    break;
                }
            }

            /* we check if the message has a data field */
            if (!jmessage.count("data"))
            {
                errorResponse(response, CODE_SIGN_UP,
                    "SIGN_UP: Invalid message format; insufficient parameters");

                break;
            }

            /* we check if the data field is an object */
            if (!jmessage["data"].is_object())
            {
                errorResponse(response, CODE_SIGN_UP,
                    "SIGN_UP: Invalid message data field type");

                break;
            }

            /* we check if the user provided an id and password */
            if (!jmessage["data"].count("userID")
                || !jmessage["data"].count("userPassword"))
            {
                errorResponse(response, CODE_SIGN_UP,
                    "SIGN_UP: Invalid message format; insufficient parameters");

                break;
            }

            /* we check the formats of the id and password */
            if (!jmessage["data"]["userID"].is_string() ||
                !jmessage["data"]["userPassword"].is_string())
            {
                errorResponse(response, CODE_SIGN_UP,
                    "SIGN_UP: Invalid message data types");

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
			    response["data"]["response"]="SIGN_UP Error during Insertion execution (login already taken)";
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
                ClientIterator client;
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
                    "CONNECT: Invalid message format; insufficient parameters");

                break;
            }

            /* we check if the data field is an object */
            if (!jmessage["data"].is_object())
            {
                errorResponse(response, CODE_CONNECT,
                    "CONNECT: Invalid message data types");

                break;
            }

            /* we check if the message contains an id and a password */
            if (!jmessage["data"].count("userID")
                || !jmessage["data"].count("userPassword"))
            {
                errorResponse(response, CODE_CONNECT,
                    "CONNECT: Invalid message format; insufficient parameters");

                break;
            }

            /* we check the type of the id and the password */
            if (!jmessage["data"]["userID"].is_string() ||
                !jmessage["data"]["userPassword"].is_string())
            {
                errorResponse(response, CODE_CONNECT,
                    "CONNECT: Invalid message data types");

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
        sqlite3_finalize(stmt);
        break;
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
          sqlite3_finalize(stmt);
          break;

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
            sqlite3_finalize(stmt);
            break;
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
                sqlite3_finalize(stmt);
                break;
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
              sqlite3_finalize(stmt);
              break;
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
                ClientIterator client;
                client = clients.find(connection.lock().get());

                /* if we can't find the user we send an error */
                if (client == clients.end())
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
                cout << "User disconnected (id = " << client->second.getName()
                    << ")" << endl;

                /* remove disconnected user from his game if he is in one */
                int gameID = client->second.getGameID();
                if (gameID != SERVER_HUB)
                {
                    GameIterator game;
                    game = games.find(gameID);

                    if (game != games.end())
                    {
                        game->second.removePlayer(client->second.getName());
                        client->second.setGameID(SERVER_HUB);
                    }
                }

                /* remove disconnected user from clients map */
                clients.erase(client);
            }

            /* construct response */
            response["type"] = CODE_DISCONNECT;
            response["data"]["error"] = false;
            response["data"]["response"] = "Success";
            break;


        case CODE_CREATE_LOBBY:

            /* we check if the message has a data field */
            if (!jmessage.count("data"))
            {
                errorResponse(response, CODE_CREATE_LOBBY,
                    "CREATE_LOBBY: Invalid message; insufficient parameters");

                break;
            }

            /* we check the data field type */
            if (!jmessage["data"].is_object())
            {
                errorResponse(response, CODE_CREATE_LOBBY,
                    "CREATE_LOBBY: Invalid message data types");

                break;
            }

            /* we check if the data field contains the necessary info */
            if (!jmessage["data"].count("lobbyName") ||
                !jmessage["data"].count("lobbyPassword") ||
                !jmessage["data"].count("maxPlayers") ||
                !jmessage["data"].count("mapName"))
            {
                errorResponse(response, CODE_CREATE_LOBBY,
                    "CREATE_LOBBY: Invalid message; insufficient parameters");

                break;
            }

            /* and we check if the info is in the correct type */
            if (!jmessage["data"]["lobbyName"].is_string() ||
                !jmessage["data"]["lobbyPassword"].is_string() ||
                !jmessage["data"]["maxPlayers"].is_number() ||
                !jmessage["data"]["mapName"].is_string())
            {
                errorResponse(response, CODE_CREATE_LOBBY,
                    "CREATE_LOBBY: Invalid message data types");

                break;
            }

            /* check if user is connected */
            {
                ClientIterator client;
                client = clients.find(connection.lock().get());

                /* if we can't find the user we send an error */
                if (client == clients.end())
                {
                    errorResponse(response, CODE_CREATE_LOBBY,
                        "CREATE_LOBBY: User not connected");
                    break;
                }

                /* we check if the user is not already in a game */
                if (client->second.getGameID() != SERVER_HUB)
                {
                    errorResponse(response, CODE_CREATE_LOBBY,
                        "CREATE_LOBBY: User already in lobby/game");
                    break;
                }

                /* logging */
                cout << "User " << client->second.getName()
                    << ": attempt to create lobby "
                    << jmessage["data"]["lobbyName"]
                    << "with password" << jmessage["data"]["lobbyPassword"]
                    << "of max players " << jmessage["data"]["maxPlayers"]
                    << "and on map " << jmessage["data"]["mapName"] << endl;


                /* we create a new game */
                int newGameId = createGame(jmessage["data"]["mapName"],
                    client->second.getName(), jmessage["data"]["maxPlayers"],
                    jmessage["data"]["lobbyName"],
                    jmessage["data"]["lobbyPassword"]);
                client->second.setGameID(newGameId);

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
                ClientIterator client;
                client = clients.find(connection.lock().get());

                /* if we can't find the user we send an error */
                if (client == clients.end())
                {
                    errorResponse(response, CODE_LOBBY_LIST,
                        "LOBBY_LIST: User not connected");
                    break;
                }

                /* check if the user is in a game */
                if (client->second.getGameID() != SERVER_HUB)
                {
                    errorResponse(response, CODE_LOBBY_LIST,
                        "LOBBY_LIST: User already in lobby/game");
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
                GameIterator game;
                for (game = games.begin(); nb < MAX_GAMES && game != games.end();
                    game++)
                {
                    if (!game->second.isRunning())
                    {
                        response["data"]["gameList"].push_back(
                            game->second.toJSON());
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
                    "JOIN_LOBBY: Invalid message; insufficient parameters");

                break;
            }

            /* we check the data field type */
            if (!jmessage["data"].is_object())
            {
                errorResponse(response, CODE_JOIN_LOBBY,
                    "JOIN_LOBBY: Invalid message data types");

                break;
            }

            /* we check if the data field contains the necessary info */
            if (!jmessage["data"].count("lobbyID") ||
                !jmessage["data"].count("lobbyPassword")
                )
            {
                errorResponse(response, CODE_JOIN_LOBBY,
                    "JOIN_LOBBY: Invalid message; insufficient parameters");

                break;
            }

            /* we check if the info has the correct type */
            if (!jmessage["data"]["lobbyID"].is_number() ||
                !jmessage["data"]["lobbyPassword"].is_string())
            {
                errorResponse(response, CODE_JOIN_LOBBY,
                    "JOIN_LOBBY: Invalid message data types");

                break;
            }

            /* check if user is connected */
            {
                ClientIterator client;
                client = clients.find(connection.lock().get());

                /* if we can't find the user we send an error */
                if (client == clients.end())
                {
                    errorResponse(response, CODE_JOIN_LOBBY,
                        "JOIN_LOBBY: User not connected");
                    break;
                }

                /* check if the user is in a game */
                if (client->second.getGameID() != SERVER_HUB)
                {
                    errorResponse(response, CODE_JOIN_LOBBY,
                        "JOIN_LOBBY: User already in lobby/game");
                    break;
                }

                /* logging */
                cout << "User " << client->second.getName()
                << " tried to join lobby with id: "
                << jmessage["data"]["lobbyID"]
                << "and password: \"" << jmessage["data"]["lobbyPassword"]
                << "\"" << endl;

                /* we look for the lobby */
                GameIterator game;
                game = games.find(jmessage["data"]["lobbyID"]);

                /* if the lobby is not found send error response */
                if (game == games.end())
                {
                    errorResponse(response, CODE_JOIN_LOBBY,
                        "JOIN_LOBBY: Lobby not found");
                    break;
                }

                /* check if the lobby is full */
                if (game->second.isFull())
                {
                    errorResponse(response, CODE_JOIN_LOBBY,
                        "JOIN_LOBBY: Lobby is full");
                    break;
                }

                /* check if the password is correct */
                if (game->second.getPassword()
                    != jmessage["data"]["lobbyPassword"])
                {
                    errorResponse(response, CODE_JOIN_LOBBY,
                        "JOIN_LOBBY: Incorrect password");
                    break;
                }

                /* if all tests pass, we can join the game */
                game->second.addPlayer(client->second.getName());
                client->second.setGameID(jmessage["data"]["lobbyID"]);

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
                    ClientIterator player;

                    // player = clients.find(connection.lock().get());
                    // Connection c = player->second.getConnection();

                    for (player = clients.begin(); player != clients.end();
                        player++)
                    {
                        if (player->second.getName() == players[i].getName())
                        {
                            Connection c = player->second.getConnection();
                            endpoint.send(c, update.dump(),
                                websocketpp::frame::opcode::text);
                        }
                    }

                }
            }
            break;

        case CODE_LEAVE_GAME:

            /* check if user is connected */
            {
                ClientIterator client;
                client = clients.find(connection.lock().get());

                if (client == clients.end())
                {
                    errorResponse(response, CODE_LEAVE_GAME,
                        "LEAVE_GAME: User not connected");
                    break;
                }

                /* check if client is in the lobby/game */
                GameIterator game;
                game = games.find(client->second.getGameID());

                /* if we can't find the game we send an error */
                if (game == games.end())
                {
                    errorResponse(response, CODE_LEAVE_GAME,
                        "LEAVE_GAME: User not in lobby");
                    break;
                }

                /* if we find the game we remove the player from the game */
                game->second.removePlayer(client->second.getName());
                client->second.setGameID(SERVER_HUB);

                /* construct the response */
                response["type"] = CODE_LEAVE_GAME;
                response["data"]["error"] = false;
                response["data"]["response"] = "Success";

                /* and if the room is empty we delete it */
                if (game->second.getNbPlayers() == 0)
                {
                    games.erase(game);
                    break;
                }

                /* if the give all existing players new lobby state */
                json update;
                update["type"] = CODE_LOBBY_STATE;
                update["data"]["error"] = false;
                update["data"]["response"] = "Success";
                update["data"]["gameData"] = game->second.toJSON();

                vector<Player> players = game->second.getPlayers();
                for (unsigned int i = 0; i < players.size(); i++)
                {
                    ClientIterator player;

                    for (player = clients.begin(); player != clients.end();
                        player++)
                    {
                        if (player->second.getName() == players[i].getName())
                        {
                            Connection c = player->second.getConnection();
                            endpoint.send(c, update.dump(),
                                websocketpp::frame::opcode::text);
                        }
                    }

                }

            }

            break;

        case CODE_LOBBY_STATE:

            /* check if user is connected */
            {
                ClientIterator client;
                client = clients.find(connection.lock().get());

                if (client == clients.end())
                {
                    errorResponse(response, CODE_LOBBY_STATE,
                        "LOBBY_STATE: User not connected");
                    break;
                }

                GameIterator game;
                game = games.find(client->second.getGameID());

                /* if we don't find the lobby we send an error */
                if (game == games.end())
                {
                    errorResponse(response, CODE_LOBBY_STATE,
                        "LOBBY_STATE: Lobby not found");
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

		case CODE_START_GAME:
    /* check if user is connected */
    {
        ClientIterator client;
        client = clients.find(connection.lock().get());

        if (client == clients.end())
        {
            errorResponse(response, CODE_START_GAME,
                "START_GAME: User not connected");
            break;
        }

        GameIterator game;
        game = games.find(client->second.getGameID());

        /* if we don't find the lobby we send an error */
        if (game == games.end())
        {
            errorResponse(response, CODE_START_GAME,
                "START_GAME: Lobby not found");
            break;
        }

        //check if the message is sent by the game owner
        if(game->second.getPlayers().at(0).getName() != client->second.getName())
        {
          errorResponse(response, CODE_START_GAME,
              "START_GAME: You are not the owner of the game");
          break;
        }

        //check if there is more than one player
        if(game->second.getPlayers().size() <2)
        {
          errorResponse(response, CODE_START_GAME,
              "START_GAME: You cannot start a game alone");
          break;
        }

        //Starting the game
        if(game->second.messageStart()==-1)
        {
          errorResponse(response, CODE_START_GAME,
              "START_GAME: The game is already started");
          break;
        }

        //Sending START_GAME and GAME_STATUS to all clients in this game
        json start;
        start["type"]=CODE_START_GAME;
        string status ="{\"type\":"+to_string(CODE_GAME_STATUS)+",\"data\":"+game->second.toJson()+"}";
        vector<Player> players = game->second.getPlayers();
        for (unsigned int i = 0; i < players.size(); i++)
        {
            ClientIterator player;

            for (player = clients.begin(); player != clients.end();
                player++)
            {
                if (player->second.getName() != client->second.getName() && player->second.getName() == players[i].getName())
                {
                    Connection c = player->second.getConnection();
                    endpoint.send(c, start.dump(),
                        websocketpp::frame::opcode::text);//sending start
                    endpoint.send(c, status,
                        websocketpp::frame::opcode::text);//sending status
                }
            }

        }

        response["type"] = CODE_START_GAME;
        Connection c = client->second.getConnection();
        endpoint.send(c, status,
            websocketpp::frame::opcode::text);//sending status to sender

    }

		break;

    case CODE_GAME_STATUS:
    {
        ClientIterator client;
        client = clients.find(connection.lock().get());

        if (client == clients.end())
        {
            errorResponse(response, CODE_GAME_STATUS,
                "GAME_STATUS: User not connected");
            break;
        }

        GameIterator game;
        game = games.find(client->second.getGameID());

        /* if we don't find the game we send an error */
        if (game == games.end())
        {
            errorResponse(response, CODE_GAME_STATUS,
                "GAME_STATUS: Game not found");
            break;
        }

        //If the game has not been started
        if(!game->second.isRunning())
        {
          errorResponse(response, CODE_GAME_STATUS,
              "GAME_STATUS: The game is not running yet");
          break;
        }

        //Sending GAME_STATUS to the client
        return "{\"type\":"+to_string(CODE_GAME_STATUS)+",\"data\":"+game->second.toJson()+"}";

    }
    break;

		case CODE_END_PHASE:
    {
        ClientIterator client;
        client = clients.find(connection.lock().get());

        if (client == clients.end())
        {
            errorMessage(response, CODE_END_PHASE,
                "END_PHASE: User not connected");
            break;
        }

        GameIterator game;
        game = games.find(client->second.getGameID());

        /* if we don't find the game we send an error */
        if (game == games.end())
        {
            errorMessage(response, CODE_END_PHASE,
                "END_PHASE: Game not found");
            break;
        }

        if(!game->second.isActivePlayer(client->second.getName()))
        {
          errorMessage(response, CODE_END_PHASE,
              "END_PHASE: It's not your turn");
          break;
        }

        if(game->second.getPhase() == -1)
        {
          errorMessage(response, CODE_END_PHASE,
              "END_PHASE: Message not alowed in phase -1");
          break;
        }

        //Message action
        int gameReturn = game->second.messageEndPhase(game->second.getPlayerOrder(client->second.getName()));
        if(gameReturn == -1)//not your turn
        {
          errorMessage(response, CODE_END_PHASE,
              "END_PHASE: It's not your turn");
          break;
        }
        if(gameReturn == -2)//reinforcements > 0 in phase 0
        {
          errorMessage(response, CODE_END_PHASE,
              "END_PHASE: You need to spend your reinforcements");
          break;
        }
        if(gameReturn == -3)//tokens >= 5 in phase 0
        {
          errorMessage(response, CODE_END_PHASE,
              "END_PHASE: You need to spend a set of tokens");
          break;
        }
        if(gameReturn == -4)//unfinished combat in phase 1
        {
          errorMessage(response, CODE_END_PHASE,
              "END_PHASE: Your combat is unfinished");
          break;
        }
        json end,rein;
        end["type"]=CODE_CURRENT_PHASE;
        end["data"]["phase"]=game->second.getPhase();
        if(game->second.getPhase() == 0)//if beginning of a new turn
        {
          rein["type"]=CODE_REINFORCEMENT;
          rein["data"]["player"]=game->second.getActivePlayer();
          rein["data"]["units"]=game->second.getPlayers().at(game->second.getActivePlayer()).getReinforcement();
        }
        vector<Player> players = game->second.getPlayers();
        for (unsigned int i = 0; i < players.size(); i++)
        {
            ClientIterator player;

            for (player = clients.begin(); player != clients.end();
                player++)
            {
                if (player->second.getName() != client->second.getName() && player->second.getName() == players[i].getName())
                {//sending to everyone but sender
                    Connection c = player->second.getConnection();
                    endpoint.send(c,end.dump(),
                        websocketpp::frame::opcode::text);//sending current_phase
                    if(game->second.getPhase() == 0)//if beginning of a new turn
                      endpoint.send(c,rein.dump(),websocketpp::frame::opcode::text);//sending reinforcement
                }
            }

        }
        if(game->second.getPhase() == 0)//if beginning of a new turn
        {
          Connection c = client->second.getConnection();
          endpoint.send(c,rein.dump(),websocketpp::frame::opcode::text);//sending reinforcement to sender
        }
        response["type"]=CODE_CURRENT_PHASE;
        response["data"]["phase"]= game->second.getPhase();


    }
    break;
		case CODE_PUT:
    {
        /* we check if the message has a data field */
        if (!jmessage.count("data"))
        {
          errorMessage(response, CODE_PUT,
              "PUT: Invalid message format");

            break;
        }

        /* we check the data field type */
        if (!jmessage["data"].is_object())
        {
          errorMessage(response, CODE_PUT,
              "PUT: Invalid message format");

            break;
        }

        /* we check if the data field contains the necessary info */
        if (!jmessage["data"].count("territory") ||
            !jmessage["data"].count("units")
            )
        {
          errorMessage(response, CODE_PUT,
              "PUT: Invalid message format, bad parameters");

            break;
        }

        /* we check if the info has the correct type */
        if (!jmessage["data"]["territory"].is_number() ||
            !jmessage["data"]["units"].is_number())
        {
            errorMessage(response, CODE_PUT,
                "PUT: Invalid message format, bad parameters");

            break;
        }

        //check if user is connected
        ClientIterator client;
        client = clients.find(connection.lock().get());

        if (client == clients.end())
        {
          errorMessage(response, CODE_PUT,
              "PUT: You are not connected");
            break;
        }

        GameIterator game;
        game = games.find(client->second.getGameID());

        /* if we don't find the game we send an error */
        if (game == games.end())
        {
          errorMessage(response, CODE_PUT,
              "PUT: You are not in a game");
            break;
        }

        //Message action

        int gameReturn = game->second.messagePut(game->second.getPlayerOrder(client->second.getName()),jmessage["data"]["territory"],jmessage["data"]["units"]);
        if(gameReturn == -1)//Not your turn or bad phase
        {
          errorMessage(response, CODE_PUT,
              "PUT: You cannot put units now");
            break;
        }
        if(gameReturn == -2)//units > 1 in phase -1
        {
          errorMessage(response, CODE_PUT,
              "PUT: You can only put one unit initialisation phase");
            break;
        }
        if(gameReturn == -3)//Not on free territory and free territories > 0
        {
          errorMessage(response, CODE_PUT,
              "PUT: You have to put the unit on a free territory");
            break;
        }
        if(gameReturn == -4)//Not your turn or bad phase
        {
          errorMessage(response, CODE_PUT,
              "PUT: You don't own this territory AND/OR don't have enough units");
            break;
        }

        //gameReturn == 0 || gameReturn >0
        json put,end,rein;
        put["type"]=CODE_PUT;
        put["data"]["player"]=game->second.getPlayerOrder(client->second.getName());
        put["data"]["territory"]=jmessage["data"]["territory"];
        put["data"]["units"]=jmessage["data"]["units"];

        if(gameReturn >0)//end of phase -1
        {
          end["type"]=CODE_CURRENT_PHASE;
          end["data"]["phase"]=game->second.getPhase();
          rein["type"]=CODE_REINFORCEMENT;
          rein["data"]["player"]=game->second.getActivePlayer();
          rein["data"]["units"]=game->second.getPlayers().at(game->second.getActivePlayer()).getReinforcement();
        }

        vector<Player> players = game->second.getPlayers();
        for (unsigned int i = 0; i < players.size(); i++)
        {
            ClientIterator player;

            for (player = clients.begin(); player != clients.end();
                player++)
            {
                if (player->second.getName() != client->second.getName() && player->second.getName() == players[i].getName())
                {//sending to everyone but sender
                    Connection c = player->second.getConnection();
                    endpoint.send(c,put.dump(),
                        websocketpp::frame::opcode::text);//sending put
                    if(gameReturn >0)
                    {
                        endpoint.send(c,end.dump(),websocketpp::frame::opcode::text);//sending current_phase
                        endpoint.send(c,rein.dump(),websocketpp::frame::opcode::text);//sending reinforcement
                    }
                }
            }

        }
        response["type"]=CODE_PUT;
        response["data"]["player"]=game->second.getPlayerOrder(client->second.getName());
        response["data"]["territory"]=jmessage["data"]["territory"];
        response["data"]["units"]=jmessage["data"]["units"];
        if(gameReturn >0)
        {
            Connection c = client->second.getConnection();
            endpoint.send(c,end.dump(),websocketpp::frame::opcode::text);//sending current_phase to sender
            endpoint.send(c,rein.dump(),websocketpp::frame::opcode::text);//sending reinforcement to sender
        }

    }
    break;
		case CODE_USE_TOKENS:
    {
      /* we check if the message has a data field */
      if (!jmessage.count("data"))
      {
        errorMessage(response, CODE_USE_TOKENS,
            "USE_TOKENS: Invalid message format");

          break;
      }

      /* we check the data field type */
      if (!jmessage["data"].is_object())
      {
        errorMessage(response, CODE_USE_TOKENS,
            "USE_TOKENS: Invalid message format");

          break;
      }

      /* we check if the data field contains the necessary info */
      if (!jmessage["data"].count("token1") ||
          !jmessage["data"].count("token2") ||
          !jmessage["data"].count("token3")
          )
      {
        errorMessage(response, CODE_USE_TOKENS,
            "USE_TOKENS: Invalid message format, bad parameters");

          break;
      }

      /* we check if the info has the correct type */
      if (!jmessage["data"]["token1"].is_number() ||
          !jmessage["data"]["token2"].is_number() ||
          !jmessage["data"]["token3"].is_number()
         )
      {
          errorMessage(response, CODE_USE_TOKENS,
              "USE_TOKENS: Invalid message format, bad parameters");

          break;
      }

      //check if user is connected
      ClientIterator client;
      client = clients.find(connection.lock().get());

      if (client == clients.end())
      {
        errorMessage(response, CODE_USE_TOKENS,
            "USE_TOKENS: You are not connected");
          break;
      }

      GameIterator game;
      game = games.find(client->second.getGameID());

      /* if we don't find the game we send an error */
      if (game == games.end())
      {
        errorMessage(response, CODE_USE_TOKENS,
            "USE_TOKENS: You are not in a game");
          break;
      }

      //Message action
      int gameReturn = game->second.messageUseTokens(game->second.getPlayerOrder(client->second.getName()),jmessage["data"]["token1"],jmessage["data"]["token2"],jmessage["data"]["token3"]);
      if(gameReturn == -1)//not your turn
      {
        errorMessage(response, CODE_USE_TOKENS,
            "USE_TOKENS: It's not your turn");
          break;
      }
      if(gameReturn == -2)//wrong phase
      {
        errorMessage(response, CODE_USE_TOKENS,
            "USE_TOKENS: You cannont use tokens now");
          break;
      }
      if(gameReturn == -3)//don't have these tokens
      {
        errorMessage(response, CODE_USE_TOKENS,
            "USE_TOKENS: You don't have this set of tokens");
          break;
      }
      if(gameReturn == -4)//The set isn't valid
      {
          errorMessage(response, CODE_USE_TOKENS,
              "USE_TOKENS: This set of tokens is not valid");
            break;
      }

      //else ok and gameReturn contains the number of reinforcements granted
      json usetoks, rein;
      usetoks["type"]=CODE_USE_TOKENS;
      usetoks["data"]["player"]=game->second.getPlayerOrder(client->second.getName());
      usetoks["data"]["token1"]=jmessage["data"]["token1"];
      usetoks["data"]["token2"]=jmessage["data"]["token2"];
      usetoks["data"]["token3"]=jmessage["data"]["token3"];
      rein["type"]=CODE_REINFORCEMENT;
      rein["data"]["player"]=game->second.getPlayerOrder(client->second.getName());
      rein["data"]["units"]=gameReturn;
      vector<Player> players = game->second.getPlayers();
      for (unsigned int i = 0; i < players.size(); i++)
      {
          ClientIterator player;

          for (player = clients.begin(); player != clients.end();
              player++)
          {
              if (player->second.getName() != client->second.getName() && player->second.getName() == players[i].getName())
              {//sending to everyone but sender
                  Connection c = player->second.getConnection();
                  endpoint.send(c,usetoks.dump(),
                      websocketpp::frame::opcode::text);//sending use_tokens
                      endpoint.send(c,rein.dump(),websocketpp::frame::opcode::text);//sending reinforcement
              }
          }

      }
      response["type"]=CODE_USE_TOKENS;
      response["data"]["player"]=game->second.getPlayerOrder(client->second.getName());
      response["data"]["token1"]=jmessage["data"]["token1"];
      response["data"]["token2"]=jmessage["data"]["token2"];
      response["data"]["token3"]=jmessage["data"]["token3"];
          Connection c = client->second.getConnection();
          endpoint.send(c,rein.dump(),websocketpp::frame::opcode::text);//sending reinforcement to sender

    }
    break;
		case CODE_ATTACK:
    {
      /* we check if the message has a data field */
      if (!jmessage.count("data"))
      {
        errorMessage(response, CODE_ATTACK,
            "ATTACK: Invalid message format");

          break;
      }

      /* we check the data field type */
      if (!jmessage["data"].is_object())
      {
        errorMessage(response, CODE_ATTACK,
            "ATTACK: Invalid message format");

          break;
      }

      /* we check if the data field contains the necessary info */
      if (!jmessage["data"].count("source") ||
          !jmessage["data"].count("destination") ||
          !jmessage["data"].count("units")
          )
      {
        errorMessage(response, CODE_ATTACK,
            "ATTACK: Invalid message format, bad parameters");

          break;
      }

      /* we check if the info has the correct type */
      if (!jmessage["data"]["source"].is_number() ||
          !jmessage["data"]["destination"].is_number() ||
          !jmessage["data"]["units"].is_number()
         )
      {
          errorMessage(response, CODE_ATTACK,
              "ATTACK: Invalid message format, bad parameters");

          break;
      }

      //check if user is connected
      ClientIterator client;
      client = clients.find(connection.lock().get());

      if (client == clients.end())
      {
        errorMessage(response, CODE_ATTACK,
            "ATTACK: You are not connected");
          break;
      }

      GameIterator game;
      game = games.find(client->second.getGameID());

      /* if we don't find the game we send an error */
      if (game == games.end())
      {
        errorMessage(response, CODE_ATTACK,
            "ATTACK: You are not in a game");
          break;
      }

      //Message action
      int gameReturn = game->second.messageAttack(game->second.getPlayerOrder(client->second.getName()),jmessage["data"]["source"],jmessage["data"]["destination"],jmessage["data"]["units"]);
      if(gameReturn == -1)//Not your turn
      {
        errorMessage(response, CODE_ATTACK,
            "ATTACK: It's not your turn");
          break;
      }
      if(gameReturn == -2)//Wrong phase
      {
        errorMessage(response, CODE_ATTACK,
            "ATTACK: You cannot attack now");
          break;
      }
      if(gameReturn == -3)//units <1 or units >3
      {
        errorMessage(response, CODE_ATTACK,
            "ATTACK: You can only attack with 1, 2 or 3 units");
          break;
      }
      if(gameReturn == -4)//combat already taking place
      {
        errorMessage(response, CODE_ATTACK,
            "ATTACK: A combat is already taking place, please wait the defender's response");
          break;
      }
      if(gameReturn == -5)//you don't own the source territory
      {
        errorMessage(response, CODE_ATTACK,
            "ATTACK: You don't own the source territory");
          break;
      }
      if(gameReturn == -6)//you own the destination
      {
        errorMessage(response, CODE_ATTACK,
            "ATTACK: You cannot attack one of your terrirories");
          break;
      }
      if(gameReturn == -7)//territories not adjacent
      {
        errorMessage(response, CODE_ATTACK,
            "ATTACK: The two territories aren't adjacent");
          break;
      }
      if(gameReturn == -8)//have to leave 1 unit on source at least
      {
        errorMessage(response, CODE_ATTACK,
            "ATTACK: You have to leave at least 1 unit on your source territory");
          break;
      }

      //else gameReturn == 0 -> ok
      vector<Player> players = game->second.getPlayers();
      json atk, ated;
      atk["type"]=CODE_ATTACK;
      atk["data"]["attackerName"]=client->second.getName();
      atk["data"]["defenderName"]=players.at(game->second.getGameBoard().at(jmessage["data"]["destination"]).owner).getName();
      atk["data"]["source"]=jmessage["data"]["source"];
      atk["data"]["destination"]=jmessage["data"]["destination"];
      atk["data"]["units"]=jmessage["data"]["units"];

      ated["type"]=CODE_ATTACKED;
      ated["data"]["targetedTerritory"]=jmessage["data"]["destination"];
      ated["data"]["units"]=jmessage["data"]["units"];
      for (unsigned int i = 0; i < players.size(); i++)
      {
          ClientIterator player;

          for (player = clients.begin(); player != clients.end();
              player++)
          {
              if (player->second.getName() != client->second.getName() && player->second.getName() == players[i].getName())
              {//sending to everyone but sender
                  Connection c = player->second.getConnection();
                  endpoint.send(c,atk.dump(),
                      websocketpp::frame::opcode::text);//sending attack
                  if((int)i == game->second.getGameBoard().at(jmessage["data"]["destination"]).owner) //if it's the attacked player
                      endpoint.send(c,ated.dump(),websocketpp::frame::opcode::text);//sending attacked
              }
          }

      }

      response["type"]=CODE_ATTACK;
      response["data"]["attackerName"]=client->second.getName();
      response["data"]["defenderName"]=players.at(game->second.getGameBoard().at(jmessage["data"]["destination"]).owner).getName();
      response["data"]["source"]=jmessage["data"]["source"];
      response["data"]["destination"]=jmessage["data"]["destination"];
      response["data"]["units"]=jmessage["data"]["units"];
    }
    break;
		case CODE_DEFEND:
		case CODE_MOVE:

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


int GameServer::createGame(string mapName, string host, int nbPlayers,
    string name, string password)
{
    /* create a new game */
    Game newGame(mapName, host, MAX_PLAYERS, name, password);

    /* copy the new object into the games list */
    int newGameID = newGame.getId();
    games.emplace(newGameID, newGame);

    return newGameID;
}


int GameServer::destroyGame(int id)
{
    GameIterator game;
    game = games.find(id);

    if (game == games.end())
    {
        cout << "Game " << id << " not found" << endl;
        return -1;
    }

    /* get players list */
    vector<Player> players = game->second.getPlayers();

    /* loop through players and remove them */
    vector<Player>::iterator player;
    for (player = players.begin(); player != players.end(); player++)
    {
        /* remove player from game */
        game->second.removePlayer(player->getName());

        /* find player in client map ... */
        ClientIterator client;
        for(client = clients.begin();
            client != clients.end() &&
            client->second.getName() != player->getName(); client++)
        {}

        if (client == clients.end())
        {
            cout << "User not found" << endl;
        }
        else
        {
            /* ... and mark him as not in room */
            client->second.setGameID(SERVER_HUB);
        }
    }

    games.erase(game);
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
        endpoint.set_message_handler(&onMessageThread);
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
    ClientIterator client;
    for(client = clients.begin(); client != clients.end(); client++)
    {
        try
        {
            endpoint.pause_reading(client->second.getConnection());
            endpoint.close(client->second.getConnection(),
                websocketpp::close::status::going_away, "Server shutdown");
        }
        catch(websocketpp::exception e)
        {
            cout << "close connection exception: " << e.what() << endl;
        }
    }

    vector<Connection>::iterator connection;
    for (connection = unregisteredConnections.begin();
        connection != unregisteredConnections.end(); connection++)
    {
        try
        {
            endpoint.pause_reading(*connection);
            endpoint.close(*connection, websocketpp::close::status::going_away,
                "Server shutdown");
        }
        catch(websocketpp::exception e)
        {
            cout << "close connection exception: " << e.what() << endl;
        }
    }
}

void GameServer::onMessageThread(Connection connection, Message msg)
{
  thread th(onMessage,connection,msg);
  th.detach();
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
    ClientIterator client;
    client = clients.find(connection.lock().get());

    if (client != clients.end())
    {
        /* check if client is in game ... */
        int clientGameID = client->second.getGameID();
        if (clientGameID != SERVER_HUB)
        {
            GameIterator game = games.find(clientGameID);
            if (game == games.end())
            {
                cout << "Game not found" << endl;
            }
            else
            {
                /* .. and if he is remove him from the game */
                game->second.removePlayer(client->second.getName());
            }
        }

        /* erase him from the clients table */
        clients.erase(client);
        found = true;
    }

    if (found)
        return;

    /* check unregistered connections */
    vector<Connection>::iterator c;
    for (c = unregisteredConnections.begin();
        c != unregisteredConnections.end() && !found; c++)
    {
        if (c->lock().get() == connection.lock().get())
        {
            unregisteredConnections.erase(c);
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

void GameServer::errorMessage(json& response, MessageCode codeThatCausedError, string errorDescription)
{
    response["type"] = CODE_ERROR;
    response["data"]["errType"]= codeThatCausedError;
    response["data"]["message"]=errorDescription;
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
