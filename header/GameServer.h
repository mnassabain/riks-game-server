#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <iostream>
#include <vector>
#include <string>
#include "../header/Game.h"

#include "../libs/websocketpp/config/asio_no_tls.hpp"
#include "../libs/websocketpp/server.hpp"


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
    static void init();
    static void run();

    /* debugging */
    static int nbGames() {return games.size();} 

};

#endif //GAMESERVER_H