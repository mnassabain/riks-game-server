#include "../header/Player.h"
using namespace std;

Player::Player(string name)
{

	/* initializing with player's name */
	this->name = name;
	/* at the beiginning of the game, every player has 0 tokens  */
	for(size_t i = 0; i < 4; i++)
	{
		this->tokens[i] = 0;
	}
	// Initialization of player status
	reinforcement = 0;
	isAlive = true;
	connected = true;
	territoriesOwned = 0;
	/* initializing of player stats */
	this->territoriesCaptured = 0;
	this->territoriesLost   = 0;
	this->setsSpent     = 0;
	this->unitsKilled=0;
	this->unitsLost=0;

}


bool Player::hasSet(int tok1, int tok2, int tok3)
{
	int toSpend[4] = { 0 };
	toSpend[tok1]++;
	toSpend[tok2]++;
	toSpend[tok3]++;

	size_t i;
	for (i = 0; i < 4; i++) {
		if (toSpend[i] > tokens[i]) {
			return false;
		}
	}

	return true;
}
int* Player::listTokens()
{

	return this->tokens;

}
void Player::removeAllTokens()
{

	for(size_t j = 0;  j < 4 ; j++)
	{
		this->tokens[j] = 0;
	}

}
void Player::receiveToken(int type)
{
	this->tokens[type]++;
}
void Player::removeToken(int type)
{
	this->tokens[type]--;
}
void Player::receiveTokens(int numberOfEachToken[4])
{
	for(size_t j = 0; j < 4 ; j++)
	{
		this->tokens[j] += numberOfEachToken[j];
	}

}
void Player::removeTokens(int numberOfEachToken[4])
{

	for(int j(0) ; j < 4 ; j++)
	{
		this->tokens[j] < numberOfEachToken[j] ? this->tokens[j] = 0 : this->tokens[j] = this->tokens[j] - numberOfEachToken[j];
	}

}

int Player::getTerritoriesCaptured()
{
	return this->territoriesCaptured;
}
int Player::getTerritoriesLost()
{
	return this->territoriesLost;
}
int Player::getSetsSpent()
{
	return this->setsSpent;
}

void Player::addTerritoriesCaptured()
{
	this->territoriesCaptured ++;
}
void Player::addTerritoriesLost()
{
	this->territoriesLost ++;
}
void Player::addSetsSpent()
{
	this->setsSpent ++;
}
/* what about the one (or those) who will get his tokens ? */
void Player::die()
{
	this->isAlive = false;
}

void Player::resurrect()
{
	this->isAlive = true;
}

void Player::addReinforcement(int reinforcement)
{
	this->reinforcement += reinforcement;
}

void Player::spendReinforcement(int reinforcement)
{
	this->reinforcement -= reinforcement;
}

void Player::resetReinforcement()
{
	this->reinforcement = 0;
}

int Player::getReinforcement()
{
	return this->reinforcement;
}

int Player::getTerritoriesOwned()
{
	return territoriesOwned;
}

void Player::addTerritoriesOwned()
{
	territoriesOwned++;
}

void Player::subTerritoriesOwned()
{
	territoriesOwned--;
}

void Player::setTerritoriesOwned(int territories)
{
	this->territoriesOwned = territories;
}

int Player::countTokensOfType(int type)
{
	return tokens[type];
}

int Player::countTokens()
{
	return tokens[0] + tokens[1] + tokens[2] + tokens[3];
}

int Player::connect()
{
	this->connected = true;
	return 0;
}

int Player::disconnect()
{
	this->connected = false;
	return 0;
}

bool Player::isConnected()
{
	return this->connected;
}

string Player::getName()
{
	return this->name;
}


string Player::toString()
{
	string status="Player name = '" + name + "', tokens[] = ";
	for(size_t i = 0; i < 4; i++)
		status += " " + tokens[i];
	status += ", isAlive = " + to_string(isAlive) + ", reinforcement = " +\
	to_string(reinforcement) + ", territoriesOwned = " +\
	to_string(territoriesOwned) + ", territoriesCaptured = " +\
	to_string(territoriesCaptured) + ", territoriesLost = " +\
	to_string(territoriesLost) + ", unitsKilled = " +\
	to_string(unitsKilled) + ", unitsLost = " +\
	to_string(unitsLost) + ", setsSpent = " + to_string(setsSpent);

	return status;
}

void Player::printPlayer()
{
	vector<string> result;
	string input = toString();
	boost::split(result, input, boost::is_any_of(","));
	cout << "\t\t" + result[0] + " : " << endl;
	for(size_t i = 1; i < result.size(); i++)
	{
		cout << "\t\t\t" + result[i] << endl;
	}
}

string Player::toJson()
{
	return "{\"name\":\""+name+"\",\"reinforcements\":"+to_string(reinforcement)+",\"tokens\":{\"tok1\":"+to_string(tokens[0])+",\"tok2\":"+to_string(tokens[1])+",\"tok3\":"+to_string(tokens[2])+",\"tok4\":"+to_string(tokens[3])+"}}";
}
