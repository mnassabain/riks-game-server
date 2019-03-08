#include "../header/Game.h"

int Game::nextId = 0;

// Private methods
void Game::start()
{
}

void Game::nextPlayer()
{
	// considering that `activePlayer` can go from 0 to `nbPlayer - 1`
	activePlayer = (activePlayer + 1) % nbPlayer;
	nextPhase();
}

void Game::nextPhase()
{
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
	this->maxPlayer = maxPlayer; // To compare and min() with map.maxPlayers once we get the getter, also consistency changes with player/players
	this->nbPlayer = 0;
	addPlayer(creatorId);

	// Initialization of game variables - Might be moved to the start() method
	this->phase = -1;
	this->activePlayer = -1;
	this->tokens[0] = 2;
	this->tokens[1] = 14;
	this->tokens[2] = 14;
	this->tokens[3] = 14;
	this->totalExchangedSets = 0;
	this->territoryCapture = false;
	this->lastAttackedTerritory = -1;
	this->lastAttackCapture = false;
	// Reminder to add the board initialization
}