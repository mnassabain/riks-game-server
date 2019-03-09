#include "../header/Player.h"
#include "../header/Player.h"
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
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
	/* getting player's stats */
	this->get_stats();
    
}
void Player::get_stats()
{
try{
	sql::Driver *driver;
	sql::Connection *con;
	sql::ResultSet *res;
	sql::PreparedStatement *pstmt;
	string request = "SELECT * FROM stats WHERE idu = ?";
	int idu = 1;

	/* Create a connection */
	driver = get_driver_instance();
	con = driver->connect("localhost:3306", "root", "test");
	/* Connect to the MySQL test database */
	con->setSchema("mytest");
	pstmt = con->prepareStatement(request);
	pstmt->setInt(1,idu);
	res = pstmt->executeQuery();
	if(res->rowsCount() == 1)
	{
		while(res->next())
		{

			this->victories          = res->getInt("victories");
			this->defeats            = res->getInt("defeats");
			this->gained_territories = res->getInt("gained_territories");
			this->lost_territories   = res->getInt("lost_territories");
			this->sets_of_tokens     = res->getInt("sets_of_tokens");

		}

	}

  	delete res;
  	delete pstmt;
  	delete con;

} 
catch (sql::SQLException &e) {

	cout << "# ERR: " << e.what();
  	cout << " (MySQL error code: " << e.getErrorCode();
  	cout << ", SQLState: " << e.getSQLState() << " )" << endl;
 
	}
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
	}
	/* updating the number of set_of_tokens */
	this->sets_of_tokens ++;
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