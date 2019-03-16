#ifndef GAMESERVER_H
#define GAMESERVER_H

using namespace std;

#include <iostream>
#include <vector>
#include <string>
#include "../header/Game.h"

#include "../libs/websocketpp/config/asio_no_tls.hpp"
#include "../libs/websocketpp/server.hpp"


#define SERVER_PORT 9002


/* typedefs for server endpoint, message pointer and connection HDL */
typedef websocketpp::server<websocketpp::config::asio> ServerEndpoint;
typedef ServerEndpoint::message_ptr Message;
typedef websocketpp::connection_hdl Connection;


class GameServer {
private:
    static vector<Game> games;

    /* sockets */
    /* server endpoint */
    static ServerEndpoint endpoint;

    /* open connection list */
    static vector<Connection> connections;

    /* message handler */
    static void onMessage(Connection connection, Message msg);

    /* open connection handler */
    static void onOpenConnection(Connection connection);

public:
    static void listen();
    static void treatMessage(string message);
    static void createGame(string mapName, vector<string> playersNames);
    static int destroyGame(int id);

    /* sockets */
    /* initialises the server endpoint */
    static void init();

    /* listens to incoming messages */
    static void run();

    /* stops server */
    static void stop();


    /* debugging */
    static int nbGames();

};

#endif //GAMESERVER_H