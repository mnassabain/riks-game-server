#include "Game.h"

Game::Game(string mapName, vector <string> playersNames){
}

void Game::start() {
}

void Game::nextPlayer() {
	//Considering that `activePlayer` can go from 0 to `nbPlayer - 1`
	activePlayer = (activePlayer + 1) % (nbPlayer - 1);
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
