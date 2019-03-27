#include "../header/Client.h"

const int Client::SERVER_HUB = -1;


Client::Client(string name, Connection connection)
{
    this->name = name;
    this->connection = connection;
    this->gameID = SERVER_HUB;
}



Connection Client::getConnection()
{
    return this->connection;
}


bool Client::inLobby()
{
    return gameID != -1;
}


string Client::getName()
{
    return this->name;
}

int Client::getGameID()
{
    return this->gameID;
}

void Client::setGameID(int gameID)
{
    this->gameID = gameID;
}