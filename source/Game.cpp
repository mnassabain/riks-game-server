#include "../header/Game.h"

int Game::nextId = 0;

// Private methods
void Game::start()
{
	// Initialization of game variables
	this->phase = -1;
	chooseFirstPlayer();
	this->tokens[0] = 2;
	this->tokens[1] = 14;
	this->tokens[2] = 14;
	this->tokens[3] = 14;
	this->totalExchangedSets = 0;
	this->territoryCapture = false;
	this->lastAttackedTerritory = -1;
	this->lastAttackCapture = false;

	// Initialization of board // I'm actually not sure if the value will be copied or referenced, so there's a potential dangerous behavior to be tested
	TerritoryState blank;
	blank.owner = -1;
	blank.units = 0;
	// Adding one blank TerritoryState per territory
	int limit = this->map.nbTerritories;
	for (int i = 0; i < limit; i++) {
		this->board.push_back(blank);
	}

	// Everything is ready to start the game, the lobby now becomes a running game
	this->isRunning = true;
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

	// Resetting the turn related variables
	this->territoryCapture = false;
	this->lastAttackedTerritory = -1;
	this->lastAttackCapture = false;
}

void Game::nextPhase()
{
	// considering that `phase` can go from 0 to 2
	this -> phase = (this -> phase + 1) % 3;
}

void Game::chooseFirstPlayer()
{
	this->activePlayer = 0; // To be replaced with a rand
}

int Game::useSet(int tok1, int tok2, int tok3)
{
	if (players[this->activePlayer].hasSet(tok1, tok2, tok3)) {
		// Removing tokens from player
		// Adding tokens back to the pool
		this->tokens[tok1]++;
		this->tokens[tok2]++;
		this->tokens[tok3]++;
		// Adding reinforcement to the active player
		this->players[this->activePlayer].addReinforcement(currentSetValue());
		// Incrementing totalExchangedSets
		this->totalExchangedSets++;
	}
	return 0;
}

int Game::currentSetValue()
{
	// Value for the first 4 exchanged sets
	if (this->totalExchangedSets < 4) {
		return 4 + 2 * this->totalExchangedSets;
	}
	// Value for all extra sets
	else {
		return 10 + 5 * (this->totalExchangedSets - 3);
	}
}

void Game::putUnits(int territory, int units)
{
	if ((this->activePlayer == this->board[this->activePlayer].owner) && (this->players[this->activePlayer].getReinforcement() >= units)) {
		this->players[this->activePlayer].spendReinforcement(units);
		this->board[territory].units += units;
	}
}

void Game::solveCombat()
{
}

void Game::moveUnits(int source, int destination, int units) // The phase checks will be performed outside, while treating messages
{
	// checking the requirements of moving units
	if(	this -> board[source].units - units >= 1 &&
		this -> activePlayer == this -> board[source].owner && 
		this -> activePlayer == this -> board[destination].owner &&
		areAdjacent(source, destination) ) { // I'd put areAdjacent as first checked condition
		this -> board[source].units -= units;
		this -> board[destination].units += units;
	}
}

void Game::setInitialReinforcement()
{
	for (int i = 0; i < this->nbPlayer; i++) {
		players[i].addReinforcement(20 + 5 * (this->map.getMaxPlayers() - this->nbPlayer)); // 20 units + 5 for each missing player
	}
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
	this->maxPlayer = min(maxPlayer, this->map.getMaxPlayers);
	this->nbPlayer = 0;
	addPlayer(creatorId);
}