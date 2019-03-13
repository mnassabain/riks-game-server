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
	/* initializing of player stats */
	this->gained_territories = 0;
	this->lost_territories   = 0;
	this->sets_of_tokens     = 0;
    
}

bool Player::set_is_valid(int tok1, int tok2, int tok3)
{
	int list_tokens[3] = {tok1, tok2, tok3};
	/* list of tokens which are not jokers */
	vector<int>res;

	/* checking if token type values are between 0 and 3 (0 and 3 included) */
	if( (tok1 < 0 || tok1 > 3) || (tok2 < 0 || tok2 > 3) || (tok3 < 0 || tok3 > 3) )
	{
		return false;
	}
	else
	{
		/* looking for the joker token  (type = 3 ) */
		for(size_t j = 0 ; j < 3; j++)
		{
			if(list_tokens[j] != 3)
			{
				res.push_back(list_tokens[j]);
			}
			
		}
		/* a set of tokens containing a joker is  valid */
		if(res.size() < 3 )
		{
			return true;
		}
		else
		{ 
			return (res[0] != res[1] && res[0] != res[2] && res[1] != res[2]) || (res[0] == res[1] && res[0] == res[2]) ;
		}
		
		
	}
	
}

bool Player::hasSet(int tok1, int tok2, int tok3)
{	
		 if(this->tokens[tok1] == 0)
		 {
			 return false;
		 }
		 if(this->tokens[tok2] == 0 )
		 {
			return false;
		 }
		 
		 if(this->tokens[tok3] == 0)
		 {
			return false;
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

int Player::get_gained_territories()
{
	return this->gained_territories;
}
int Player::get_lost_territories()
{
	return this->lost_territories;
}
int Player::get_sets_of_tokens()
{
	return this->sets_of_tokens;
}

void Player::add_gained_territories()
{
	this->gained_territories ++;
}
void Player::add_lost_territories()
{
	this->lost_territories ++;
}
void Player::add_sets_of_tokens()
{
	this->sets_of_tokens ++;
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
