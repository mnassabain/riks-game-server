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
    static vector<Connection> connections;

    /* server control methods */
    static void init();
    static void run();
    static void stop();

    /* connection & communication handlers */
    static void onMessage(Connection connection, Message msg);
    static void onOpenConnection(Connection connection);
    static void onCloseConnection(Connection connection);



public:
    /* server control methods */
    static void listen();
    static string treatMessage(string message);

    /* game control methods */
    static void createGame(string mapName, string host, int nbPlayers);
    static int destroyGame(int id);

    /* debugging */
    static int nbGames();

};

#endif // GAMESERVER_H