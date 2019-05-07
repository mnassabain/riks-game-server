/**
 * @file GameServer.h
 * @brief Main class of the server, manages the communication with a client
 */

#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <algorithm>
#include "../header/Game.h"
#include "../header/MessageCode.h"
#include "../header/Client.h"
#include "../header/sha256.h"

#include "../libs/websocketpp/config/asio_no_tls.hpp"
#include "../libs/websocketpp/server.hpp"

#include "../libs/json/json.hpp"
#include "../libs/sqlite/sqlite3.h"
#include <string.h>

using namespace std;
using json = nlohmann::json;

#define SERVER_PORT 9002

/* typedefs for server endpoint, message pointer and connection HDL */
typedef websocketpp::server<websocketpp::config::asio> ServerEndpoint;
typedef ServerEndpoint::message_ptr Message;
typedef websocketpp::connection_hdl Connection;

/* typedefs for iterators */
typedef map<int, Game>::iterator GameIterator;
typedef map<void*, Client>::iterator ClientIterator;

/**
 * @brief Main class of the server, manages the communication with a client
 * @class GameServer GameServer.h "GameServer"
 * 
 */
class GameServer {
private:
    /* == game data == */

    /** id of the server hub (no lobby) **/
    static const int SERVER_HUB;

    /** map containing all active games **/
    static map<int, Game> games;

    /* == server & socket data == */
    /** Server endpoint **/
    static ServerEndpoint endpoint;

    /** Map containing all clients connected with id and password **/
    static map<void*, Client> clients;

    /** Map containing all clients not connected with id and password **/
    static vector<Connection> unregisteredConnections;

    /* == server control methods == */

    /**
	 * @brief Initialises the websocketpp server
	 *
	 */
    static void init();

    /**
	 * @brief Starts accepting new connections & messages
	 *
	 */
    static void run();

    /**
	 * @brief Stops server and closes all connections
	 *
	 */
    static void stop();

    /* == connection & communication handlers == */

    /**
     * @brief Function calling the message handler in a new thread
     * @param connection The connection whiwh sent the message
     * @param msg The reveived message
     */
    static void onMessageThread(Connection connection, Message msg);

    /**
	 * @brief Message receive handler
	 *
	 * @param connection The connection which sent the message
	 * @param msg The received message
	 */
    static void onMessage(Connection connection, Message msg);

    /**
	 * @brief Open connection handler
	 *
	 * @param connection The connection that is opened
	 */
    static void onOpenConnection(Connection connection);

    /**
	 * @brief Close connection handler
	 *
	 * @param connection The connection that is closed
	 */
    static void onCloseConnection(Connection connection);

    /* == other communication methods == */

    /**
	 * @brief Constructs a json error response
	 *
	 * @param response The response structure to fill out
	 * @param code The message code
	 * @param message The message to send
	 */
    static void errorResponse(json& response, MessageCode code, string message);

  /**
	 * @brief errorMessage : Constructs a json error message
	 *
	 * @param response The response structure to fill out
	 * @param codeThatCausedError The message code that caused an error
	 * @param errorDescription The descritpion of the error
	 */
    static void errorMessage(json& response, MessageCode codeThatCausedError, string errorDescription);


public:
    /* == server control methods == */

    /**
	 * @brief Starts server and starts accepting connections & messages
	 *
	 */
    static void listen();

    /**
	 * @brief Treats an incoming message
	 *
	 * @param message The received message
	 * @param connection The connection which sent the message
	 * @return string The JSON format response (string)
	 */
    static string treatMessage(string message, Connection connection);

    /* == game control methods == */

    /**
	 * @brief Creates a new game/lobby
	 *
	 * @param mapName The map name
	 * @param host The game creator
	 * @param nbPlayers The maximum number of players
	 * @param name The lobby name
	 * @param password The lobby password
	 * @return int The ID of the newly created game/lobby
	 */
    static int createGame(string mapName, string host, int nbPlayers,
		string name, string password);

    /**
	 * @brief Destroys a game/lobby
	 *
	 * @param id The id of the game to destroy
	 * @return int 0 on success, -1 on error
	 */
    static int destroyGame(int id);

    /* == logging == */

    /**
	 * @brief Returns the number of active games/lobbies
	 *
	 * @return int
	 */
    static int nbGames();

    /**
	 * @brief Returns the number of connections
	 *
	 * @return int
	 */
    static int nbConnections();

    /**
	 * @brief Returns the number of clients connected with id & password
	 *
	 * @return int
	 */
    static int nbClients();

    /**
	 * @brief Returns the number of clients not connection with id & password
	 *
	 * @return int
	 */
    static int nbUnregisteredConnections();

};

#endif // GAMESERVER_H
