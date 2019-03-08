#include "../header/Player.h"

using namespace std;


Player::Player(string name)
{

	this->name = name;
	this->tokens[0] = 0;
	this->tokens[1] = 0;
	this->tokens[2] = 0;
	this->tokens[3] = 0;

    
}
bool Player::set_is_valid(int tok1, int tok2, int tok3)
{
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
	//check if set of tokens is valid
	if(set_is_valid(tok1,tok2,tok3))
	{
		// build set of tokens
		this->set_of_tokens[0] = tok1;
		this->set_of_tokens[1] = tok2;   
		this->set_of_tokens[2] = tok3;   
		    
		//remove tokens from the list of tokens   
		this->tokens[tok1] > 0 ? this->tokens[tok1] = 0 : this->tokens[tok1]--;
		this->tokens[tok2] > 0 ? this->tokens[tok2] = 0 : this->tokens[tok2]--;
		this->tokens[tok3] > 0 ? this->tokens[tok3] = 0 : this->tokens[tok3]--;
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
