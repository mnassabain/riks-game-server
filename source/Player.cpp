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
	territoriesOwned = 0;
	/* initializing of player stats */
	this->territoriesCaptured = 0;
	this->territoriesLost   = 0;
	this->setsSpent     = 0;
    
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

void Player::addReinforcement(int reinforcement)
{
	this->reinforcement += reinforcement;
}

void Player::spendReinforcement(int reinforcement)
{
	this->reinforcement -= reinforcement;
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

int Player::countTokensOfType(int type)
{
	return tokens[type];
}

int Player::countTokens()
{
	return tokens[0] + tokens[1] + tokens[2] + tokens[3];
}

string Player::getName()
{
	return this->name;
}
