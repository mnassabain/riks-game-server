#include "../header/Game.h"

int Game::nextId = 0;

// Private methods
void Game::start()
{
}

void Game::nextPlayer()
{
	// considering that `activePlayer` can go from 0 to `nbPlayer - 1`
	int idPlayer = (this -> activePlayer + 1) % this -> nbPlayer;

	// don't pass the turn to an eliminated player
	while (!this -> players[idPlayer].isAlive) {
		idPlayer = (this -> activePlayer + 1) % this -> nbPlayer;
	}

	if(idPlayer == this -> activePlayer) {
		// the game ends in that case, nobody is alive except the activePlayer
		this->end();
	}
	else {
		// we move to the next phase
		this -> activePlayer = idPlayer;
		nextPhase();
	}
}

void Game::nextPhase()
{
	// considering that `phase` can go from 0 to 2
	this -> phase = (this -> phase + 1) % 3;
	// is this method where we start to implement some game logic?
}

void Game::chooseFirstPlayer()
{
}

int Game::useSet(int tok1, int tok2, int tok3)
{
	return 0;
}

void Game::solveCombat()
{
}

void Game::moveUnits(int source, int destination, int units)
{
	// checking the requirements of moving units
	if(	this -> board[source].units > 1 &&
		this -> activePlayer == this -> board[source].owner && 
		this -> activePlayer == this -> board[destination].owner &&
		areAdjacent(source, destination) ) {
		this -> board[source].units -= units;
		this -> board[destination].units += units;
	}
}

void Game::setInitialReinforcement()
{
}

void Game::end()
{
}

int Game::updatePlayersStatsInDB()
{
	return 0;
}

bool Game::areAdjacent(int a, int b) {
	vector<int> aNeighbors = this -> map.getTerritories()[a].neighbors;

	// checking if `b` is in the `neighbors` vector of the `a` territory
	if(std::find(aNeighbors.begin(), aNeighbors.end(), b) != aNeighbors.end()) {
    	return true;
	} 
	else 
		return false;
}

// Private constructors
Game::Game(string mapName, vector <string> playersNames)
{
	this->id = nextId;
	nextId++;
	this->totalExchangedSets = 0;
	this->activePlayer = -1;
	this->phase = -1;
	this->territoryCapture = false;
	this->running = false;
	this->nbPlayer = playersNames.size();
	this->map = Map::loadMap(mapName);

	// filling the `players` vector through the `playersNames` vector
	for (int i = 0; i < this->nbPlayer; i++)
	{
		Player a(playersNames[i]);
		this->players.push_back(a);
	}
}

// Public methods
bool Game::isRunning()
{
	return this->running;
}

void Game::addPlayer(string name)
{
	if ((this->nbPlayer < this->maxPlayer) && (this->running == false))
	{
		this->nbPlayer++;
		this->players.push_back(Player(name));
	}
}

int Game::getId()
{
	return this->id;
}

// Public constructors
Game::Game(string mapName, string creatorId, int maxPlayer)
{
	// Setting up game ID
	this->id = nextId;
	nextId++;

	// Loading up map
	this->map = Map::loadMap(mapName);

	// Initialization of lobby variables
	this->running = false;

	// Initialization of players
	this->maxPlayer = maxPlayer; // To compare with map max players
	this->nbPlayer = 0;
	addPlayer(creatorId);

	// Initialization of game variables
	this->phase = -1;
	this->activePlayer = -1;
	this->totalExchangedSets = 0;
	this->territoryCapture = false;
	this->lastAttackedTerritory = -1;
	this->lastAttackCapture = false;
}