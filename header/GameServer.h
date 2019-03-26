#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <iostream>
#include <vector>
#include <string>
#include "../header/Game.h"
#include "../header/MessageCode.h"

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


class GameServer {
private:
    /* game data */
    static vector<Game> games;

    /* server & socket data */
    static ServerEndpoint endpoint;
    static map<string, Connection> clients;
    static vector<Connection> unregisteredConnections;

    /* server control methods */
    static void init();
    static void run();
    static void stop();

    /* connection & communication handlers */
    static void onMessage(Connection connection, Message msg);
    static void onOpenConnection(Connection connection);
    static void onCloseConnection(Connection connection);

    /* other communication methods */
    static void errorResponse(json& response, MessageCode code, string message);


public:
    /* server control methods */
    static void listen();
    static string treatMessage(string message, Connection connection);

    /* game control methods */
    static int createGame(string mapName, string host, int nbPlayers);
    static int destroyGame(int id);

    /* debugging */
    static int nbGames();
    static int nbConnections();
    static int nbClients();
    static int nbUnregisteredConnections();

};

#endif // GAMESERVER_H
