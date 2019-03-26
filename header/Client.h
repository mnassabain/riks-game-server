#ifndef CLIENT_H
#define CLIENT_H

#include "../libs/websocketpp/config/asio_no_tls.hpp"
#include "../libs/websocketpp/server.hpp"

using namespace std;

typedef websocketpp::connection_hdl Connection;

class Client
{
    private:
        static const int SERVER_HUB;

        int gameID;
        Connection connection;

    public:
        Client(Connection connection);

        bool isInGame();
        bool isInLobby();

        Connection getConnection();
};

#endif // CLIENT_H