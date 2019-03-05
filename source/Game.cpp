#include "Game.h"

Game::Game(string mapName, vector <string> playersNames){
	// what to do with the IDs?
	this->totalExchangedSets = 0;
	this->activePlayer = -1;
	this->phase = -1;
	this->territoryCapture = false;
	this->running = false;
	this->nbPlayer = playersNames.size();

	// filling the `players` vector through the `playersNames` vector
	for(int i = 0; i < this->nbPlayer; i++)
	{
		Player a(playersNames[i]);
		this->players.push_back(a);
	}
}

void Game::start() {
}

void Game::nextPlayer() {
	// considering that `activePlayer` can go from 0 to `nbPlayer - 1`
	activePlayer = (activePlayer + 1) % nbPlayer;
	nextPhase();
}

void Game::nextPhase() {
}

void Game::chooseFirstPlayer() {
}

int Game::useSet(int tok1, int tok2, int tok3) {
	return 0;
}

void Game::solveCombat() {
}

void Game::moveUnits(int source, int destination, int units) {
}

void Game::setInitialReinforcement() {
}

void Game::end() {
}

int Game::updatePlayersStatsInDB() {
	return 0;
}
