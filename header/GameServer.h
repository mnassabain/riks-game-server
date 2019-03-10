#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <iostream>
#include <vector>
#include <string>
#include "../header/Game.h"

#include "../libs/websocketpp/config/asio_no_tls.hpp"
#include "../libs/websocketpp/server.hpp"


#define SERVER_PORT 9002


class GameServer {
private:
    static std::vector<Game> games;

    /* sockets */
    /* typedefs for server endpoint, message pointer and connection HDL */
    typedef websocketpp::server<websocketpp::config::asio> ServerEndpoint;
    typedef ServerEndpoint::message_ptr MessagePtr;
    typedef websocketpp::connection_hdl ConnectionHDL;
    
    /* server endpoint */
    static ServerEndpoint endpoint;

    /* message handler */
    static void onMessage(ConnectionHDL hdl, MessagePtr msg);

public:
    static void listen();
    static void treatMessage(std::string message);
    static void createGame(std::string mapName, std::vector<std::string> playersNames);
    static int destroyGame(int id);

    /* sockets */
    /* initialises the server endpoint */
    static void init();

    /* listens to incoming messages */
    static void run();


    /* debugging */
    static int nbGames();

};

#endif //GAMESERVER_H