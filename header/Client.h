/**
 * @file Client.h
 * @brief Utility class that stores Game/lobby info for a client
 */

#ifndef CLIENT_H
#define CLIENT_H

#include <string>

#include "../libs/websocketpp/config/asio_no_tls.hpp"
#include "../libs/websocketpp/server.hpp"

using namespace std;

typedef websocketpp::connection_hdl Connection;

/**
 * @brief Utility class that stores Game/lobby info for a client
 * @class Client Client.h "Client"
 */
class Client
{
    private:
        static const int SERVER_HUB;

        string name;
        int gameID;
        Connection connection;

    public:
        /**
         * @brief Construct a new Client object
         * 
         * @param name 
         * @param connection 
         */
        Client(string name, Connection connection);

        /**
         * @brief Checks if the client is in a lobby
         * 
         * @return true : the client is in a lobby
         * @return false : the client isn't in a lobby
         */
        bool inLobby();

        /**
         * @brief Get the Connection object
         * 
         * @return Connection 
         */
        Connection getConnection();

        /**
         * @brief Get the Name attribute
         * 
         * @return string 
         */
        string getName();

        /**
         * @brief Get the Game ID attribute
         * 
         * @return int : GameID
         */
        int getGameID();

        /**
         * @brief Set the Game ID attribute
         * 
         * @param gameID 
         */
        void setGameID(int gameID);
};

#endif // CLIENT_H