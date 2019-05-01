/**
 * @file Client.h
 * @brief 
 */

#ifndef CLIENT_H
#define CLIENT_H

#include <string>

#include "../libs/websocketpp/config/asio_no_tls.hpp"
#include "../libs/websocketpp/server.hpp"

using namespace std;

typedef websocketpp::connection_hdl Connection;


class Client
{
    private:
        static const int SERVER_HUB;

        string name;
        int gameID;
        Connection connection;

    public:
        Client(string name, Connection connection);

        bool inLobby();

        Connection getConnection();
        string getName();
        int getGameID();

        void setGameID(int gameID);
};

#endif // CLIENT_H