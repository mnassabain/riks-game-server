#include "../header/Client.h"

const int Client::SERVER_HUB = -1;


Client::Client(Connection connection)
{
    this->connection = connection;
    this->gameID = SERVER_HUB;
}



Connection Client::getConnection()
{
    return this->connection;
}