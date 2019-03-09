#include "../header/Player.h"
using namespace std;

Player::Player(string name)
{

	/* initializing with player's id */
	this->name = name;
	/* at the beiginning of the game, every player has 0 tokens  */
	for(size_t i = 0; i < 4; i++)
	{
		this->tokens[i] = 0;
	}
	/* initializing with player stats */
	this->victories          = 0;
	this->defeats            = 0;
	this->gained_territories = 0;
	this->lost_territories   = 0;
	this->sets_of_tokens     = 0;
    
}

bool Player::set_is_valid(int tok1, int tok2, int tok3)
{
	if(tok1 < 0 || tok2 < 0 || tok3 < 0)
	{
		return false;
	}
	return (tok1 != tok2 && tok1 != tok3 && tok2 != tok3) || (tok1 == tok2 && tok1 == tok3) ;
}
void Player::give_reinforcement(int number_of_rein, int last_rein_val)
{
	if(number_of_rein < 1)
	{
		this->reinforcement = 4 ;
	}
	else if(number_of_rein > 1 && number_of_rein < 4)   
	{
		this->reinforcement = last_rein_val + 2 ;   
	}
	else
	{
		this->reinforcement = last_rein_val + 5 ;
	}
}
void Player::hasSet(int tok1, int tok2, int tok3)
{
	/* check if set of tokens is valid */
	if(set_is_valid(tok1,tok2,tok3))
	{
		/* build set of tokens */
		this->set_of_tokens[0] = tok1;
		this->set_of_tokens[1] = tok2;   
		this->set_of_tokens[2] = tok3;   
		    
		/* remove tokens from the list of tokens */
		this->tokens[tok1] > 0 ? this->tokens[tok1] = 0 : this->tokens[tok1]--;
		this->tokens[tok2] > 0 ? this->tokens[tok2] = 0 : this->tokens[tok2]--;
		this->tokens[tok3] > 0 ? this->tokens[tok3] = 0 : this->tokens[tok3]--;

		/* updating the number of set_of_tokens */
		this->sets_of_tokens ++;
	}

}
vector<int>Player::listTokens()   
{

	return this->tokens;

}
void Player::removeAllTokens()
{

	for(size_t j = 0;  j < tokens.size() ; j++)
	{
		this->tokens[j] = 0;
	}

	this->isAlive = false;

}
void Player::receiveToken(int type)
{
	this->tokens[type]++;
}
void Player::receiveTokens(int numberOfEachToken[4])
{
	for(size_t j = 0; j < this->tokens.size() ; j++)
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
void Player::die()
{
	this->isAlive = false;
}
int Player::get_victories()
{
	return this->victories;
}
int Player::get_defeats()
{
	return this->defeats;
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
void Player::set_victories()
{
	this->victories ++;
}
void Player::set_defeats()
{
	this->defeats ++;
}
void Player::set_gained_territories()
{
	this->gained_territories ++;
}
void Player::set_lost_territories()
{
	this->lost_territories ++;
}
void Player::set_sets_of_tokens()
{
	this->sets_of_tokens ++;
}
