#include "../header/Game.h"

int Game::nextId = 0;

// Private methods
void Game::start()
{
	//Initialization of RNG
	srand(time(NULL));
	// Initialization of game variables
	this->phase = -1;
	chooseFirstPlayer();
	this->tokens[0] = 2;
	this->tokens[1] = 14;
	this->tokens[2] = 14;
	this->tokens[3] = 14;
	this->totalExchangedSets = 0;
	this->eliminationCount = 0;
	// Initialization of turn variables
	resetTurnVariables();
	// Initialization of combat handler
	resetCombat();

	// Initialization of board // I'm actually not sure if the value will be copied or referenced, so there's a potential dangerous behavior to be tested
	TerritoryState blank;
	blank.owner = -1;
	blank.units = 0;
	// Adding one blank TerritoryState per territory
	int limit = this->map.nbTerritories();
	freeTerritories = limit;
	for (int i = 0; i < limit; i++) {
		this->board.push_back(blank);
	}
	if(name=="riksForever")//REMOVE_AFTER_DEBUG
	{
		int tmp[4]={2,4,3,0};//REMOVE_AFTER_DEBUG
		players.at(0).receiveTokens(tmp);//REMOVE_AFTER_DEBUG
	}
	// Everything is ready to start the game, the lobby now becomes a running game
	this->running = true;
}

// Returns new activePlayer
int Game::nextPlayer()
{
	// considering that `activePlayer` can go from 0 to `nbPlayers - 1`
	int idPlayer = (this->activePlayer + 1) % this->nbPlayers;

	// don't pass the turn to an eliminated player
	while (!this->players[idPlayer].isAlive) {
		idPlayer = (this->activePlayer + 1) % this->nbPlayers;
	}

	if (idPlayer == this->activePlayer) {
		// the game ends in that case, nobody is alive except the activePlayer
		this->end();
	}
	else {
		// we move to the next player
		this->activePlayer = idPlayer;
	}

	// Resetting the turn related variables
	resetTurnVariables();

	return activePlayer;
}

int Game::nextPhase()
{
	// considering that `phase` can go from 0 to 2
	this->phase = (this->phase + 1) % 3;
	
	return this->phase;
}

void Game::chooseFirstPlayer()
{
	this->activePlayer = rand() % (this->nbPlayers);
}

int Game::turnReinforcement()
{
	int reinforcement = 0;
	int nbContinents = map.nbContinents();

	// +1 unit for each set of 3 owned territories, with a minimum of 3
	reinforcement += max(players[activePlayer].getTerritoriesOwned() / 3, 3);

	// Checks if the player has continents conquered, if so, add him the reinforcements bonus
	for (int i = 0; i < nbContinents; i++)
	{
		if (continentOwner(i) == activePlayer)
			reinforcement += this->map.getContinentById(i).bonus;
	}

	players[activePlayer].addReinforcement(reinforcement);

	return reinforcement;
}

int Game::useSet(int tok1, int tok2, int tok3)
{
	int result;
	if (players[this->activePlayer].hasSet(tok1, tok2, tok3)) {
		// Removing tokens from player
		players[this->activePlayer].removeToken(tok1);
		players[this->activePlayer].removeToken(tok2);
		players[this->activePlayer].removeToken(tok3);
		// Adding tokens back to the pool
		this->tokens[tok1]++;
		this->tokens[tok2]++;
		this->tokens[tok3]++;
		// Adding reinforcement to the active player
		result = currentSetValue();
		this->players[this->activePlayer].addReinforcement(result);
		// Incrementing totalExchangedSets
		this->totalExchangedSets++;
	}
	else return -3;

	return result;
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

bool Game::isValidSet(int tok1, int tok2, int tok3)
{
	// Basic check
	if ((tok1 < 0) || (tok1 > 3)) return false;
	if ((tok2 < 0) || (tok2 > 3)) return false;
	if ((tok3 < 0) || (tok3 > 3)) return false;

	int tokens[4] = { 0 };
	int i;
	tokens[tok1]++;
	tokens[tok2]++;
	tokens[tok3]++;

	// Set is valid if there's at least one wildcard
	if (tokens[0] > 0) return true;

	// Set is valid if there's 3 of any of the tokens
	for (i = 1; i <= 3; i++) {
		if (tokens[i] == 3) return true;
	}

	// Set is valid if all 3 tokens have a 1 value. This works considering the previous checks.
	for (i = 1; i <= 3; i++) {
		if (tokens[i] != 1) return false;
	}
	return true;
}

int Game::grantToken()
{
	// Calculating the total number of tokens available
	int total = 0;
	total += tokens[0];
	total += tokens[1];
	total += tokens[2];
	total += tokens[3];
	int token = -1 ;
	// Granting a random token to the player
	if (total > 0) {
		int roll = rand() % total;

		// Determining what token the roll refers to
		if (roll < tokens[0]) {
			token = 0;
		}
		else if (roll < tokens[0] + tokens[1]) {
			token = 1;
		}
		else if (roll < tokens[0] + tokens[1] + tokens[2]) {
			token = 2;
		}
		else token = 3;

		// Removing the token from the pool and crediting it to the player
		tokens[token]--;
		players[activePlayer].receiveToken(token);
	}
	return token;
}

int Game::putUnits(int territory, int units)
{
	if ((this->activePlayer == this->board[territory].owner) && (this->players[this->activePlayer].getReinforcement() >= units)) {
		this->players[this->activePlayer].spendReinforcement(units);
		this->board[territory].units += units;
		return 0;
	}
	else return -4;
}

CombatOutcome Game::solveCombat(int attackers, int defenders)
{
	CombatOutcome result;
	result.attackerLoss = 0;
	result.defenderLoss = 0;

	result.outcomeType = 0;
	result.source = -1;
	result.destination = -1;

	int limit = pow(6, attackers + defenders);
	int roll = rand() % limit; // Effective range : 0 to limit-1

	// Calculating unit loss for all 6 possible combat setups
	// The math behind it was done beforehand to avoid simulating multiple dice rolls, sorting them, and comparing them
	// 3 Attackers
	if (attackers == 3) {
		// 2 defenders
		if (defenders == 2) {
			if (roll < 2275) result.attackerLoss++;
			else result.defenderLoss++;

			if (roll < 2275 + 2611) result.attackerLoss++;
			else result.defenderLoss++;
		}
		// 1 defender
		if (defenders == 1) {
			if (roll < 441) result.attackerLoss++;
			else result.defenderLoss++;
		}
	}

	// 2 Attackers
	if (attackers == 2) {
		// 2 defenders
		if (defenders == 2) {
			if (roll < 581) result.attackerLoss++;
			else result.defenderLoss++;

			if (roll < 581 + 420) result.attackerLoss++;
			else result.defenderLoss++;
		}
		// 1 defender
		if (defenders == 1) {
			if (roll < 91) result.attackerLoss++;
			else result.defenderLoss++;
		}
	}

	// 1 Attacker
	if (attackers == 1) {
		// 2 defenders
		if (defenders == 2) {
			if (roll < 161) result.attackerLoss++;
			else result.defenderLoss++;
		}
		// 1 defender
		if (defenders == 1) {
			if (roll < 21) result.attackerLoss++;
			else result.defenderLoss++;
		}
	}

	return result;
}

// -6 invalid move
int Game::moveUnits(int source, int destination, int units) // The phase checks will be performed outside, while treating messages
{
	// checking the requirements of moving units
	if (areAdjacent(source, destination) &&
		this->board[source].units - units >= 1 &&
		this->activePlayer == this->board[source].owner &&
		this->activePlayer == this->board[destination].owner) {
		this->board[source].units -= units;
		this->board[destination].units += units;
	}
	else return -6;

	return 0;
}

int Game::setInitialReinforcement()
{
	// We can't take the Game's instance attribute of maxPlayer
	// otherwise we would end up with 20 reinforcements for
	// each player when playing in a 1v1 map for example
	int maxPlayersRisk = 6;
	int reinforcement = 20 + 5 * (maxPlayersRisk - this->nbPlayers); // 20 units + 5 for each missing player
	if(map.getName()=="1v1test")//REMOVE_AFTER_DEBUG
		reinforcement=5;//REMOVE_AFTER_DEBUG

	for (int i = 0; i < this->nbPlayers; i++) {
		players[i].addReinforcement(reinforcement);
	}

	return reinforcement;
}

void Game::end() //
{
}

int Game::updatePlayersStatsInDB() //
{
	return 0;
}

bool Game::areAdjacent(int a, int b)
{
	vector<int> aNeighbors = this->map.getTerritories()[a].neighbors; // A getTerritoryByID() would be appropriate

	// checking if `b` is in the `neighbors` vector of the `a` territory
	if (std::find(aNeighbors.begin(), aNeighbors.end(), b) != aNeighbors.end()) {
		return true;
	}
	else
		return false;
}

// Will return -1 if the continent has no owner
int Game::continentOwner(int idContinent)
{
	// Setting up the lower and upper limits
	int firstTerritory = this->map.getContinentById(idContinent).firstTerritory;
	int lastTerritory = this->map.getContinentById(idContinent).lastTerritory;

	// Checking who's the owner of the continent
	int owner = board[firstTerritory].owner;

	for (int i = firstTerritory + 1; i <= lastTerritory; i++) {
		if (board[i].owner != owner) {
			return -1;
		}
	}

	return owner;
}

void Game::resetCombat()
{
	this->combat.attackerId = -1;
	this->combat.defenderId = -1;
	this->combat.source = -1;
	this->combat.destination = -1;
	this->combat.attackerUnits = -1;
	this->combat.defenderUnits = -1;
}

void Game::resetTurnVariables()
{
	this->territoryCapture = false;
	this->lastAttackingTerritory = -1;
	this->lastAttackedTerritory = -1;
	this->lastAttackCapture = false;
	this->moved = false;
}

// Public methods

json Game::toJSON()
{
	json j;

	j["lobbyName"] = this->name;
	j["lobbyID"] = this->id;
	j["password"] = this->password;
	j["nbPlayers"] = this->nbPlayers;
	j["maxPlayers"] = this->maxPlayers;
	j["mapName"] = this->map.getName();
	for (int i = 0; i < this->nbPlayers; i++) {
		j["playerNames"].push_back(players[i].getName());
	}
	return j;
}

bool Game::isRunning()
{
	return this->running;
}

bool Game::isFull() {
	return (this->nbPlayers == this->maxPlayers);
}

bool Game::isActivePlayer(string name)
{
	return (getPlayerOrder(name) == activePlayer);
}

int Game::addPlayer(string name)
{
	if ((this->nbPlayers < this->maxPlayers) && (this->running == false))
	{
		this->nbPlayers++;
		this->players.push_back(Player(name));
		return 0;
	}
	return -1;
}

int Game::removePlayer(string name)
{
	size_t i;
	size_t max = players.size();
	for (i = 0; (i < max) && (players[i].getName().compare(name) != 0); i++) {
	}
	if (i < max) {
		this->nbPlayers--;
		players.erase(players.begin() + i);
		return 0;
	}
	return -1;
}

vector<int> Game::getPlayerTerritories(int player)
{
	vector<int> territories;
	for(int i = 0; i < map.nbTerritories(); i++)
	{
		if(player == board[i].owner)
			territories.push_back(i);
	}
	return territories;
}

// Will return -1 if the player isn't in this game/lobby
int Game::getPlayerOrder(string name)
{
	size_t i;
	size_t max = players.size();
	for (i = 0; i < max; i++) {
		if (players[i].getName().compare(name) == 0) return i;
	}
	return -1;
}

int Game::getId()
{
	return this->id;
}

int Game::getNbPlayers()
{
	return this->nbPlayers;
}

int Game::getActivePlayer()
{
	return this->activePlayer;
}

vector<TerritoryState> Game::getGameBoard()
{
	return this->board;
}

string Game::getPassword()
{
	return this->password;
}

int Game::getPhase()
{
	return this->phase;
}

vector<Player> Game::getPlayers()
{
	return this->players;
}


string Game::toString()
{
	string status = "lobby name = '" + name\
		+ "', id = " + to_string(id) + ", password = '" + password\
		+ "', maxPlayers = " + to_string(maxPlayers)\
		+ ", nbPlayers = " + to_string(nbPlayers) + ", running = "\
		+ to_string(running) + ", phase = " + to_string(phase)\
		+ ", activePlayer = " + to_string(activePlayer)\
		+ ", freeTerritories = " + to_string(freeTerritories)\
		+ ", territoryCapture = " + to_string(territoryCapture)\
		+ ", lastAttackedTerritory = " + to_string(lastAttackedTerritory)\
		+ ", lastAttackCapture = " + to_string(lastAttackCapture);
	// cout << "board = " << this -> board;
	// cout << "players = " << this -> players;
	return status;
}

void Game::printGame()
{
	// Printing Game::toString info
	vector<string> gameInfo;
	string gameInput = toString();
	boost::split(gameInfo, gameInput, boost::is_any_of(","));
	cout << gameInfo[0] + " : " << endl;
	for (size_t i = 1; i < gameInfo.size(); i++)
		cout << "\t" + gameInfo[i] << endl;

	// Printing board info
	cout << "\tboard[] = ";
	for (size_t i = 0; i < board.size(); i++)
	{
		if (i % 2 == 0)
		{
			cout << endl;
			cout << "\t\t";
		}
		cout << " ts" + to_string(i) + "{" + board[i].toString()\
			+ "}";
	}

	cout << endl;

	// Printing players info
	cout << "\tplayers[] :" << endl;
	for (size_t i = 0; i < players.size(); i++)
	{
		players[i].printPlayer();
		cout << endl;
	}
}
// Public constructors
Game::Game(string mapName, string creatorId, int maxPlayers)
{
	// Setting up game ID
	this->id = nextId;
	nextId++;

	// Loading up map
	this->map = Map::loadMap(mapName);

	// Initialization of lobby variables
	this->running = false;

	// Initialization of players
	this->activePlayer = -1;
	this->maxPlayers = min(maxPlayers, this->map.getMaxPlayers());
	this->nbPlayers = 0;
	addPlayer(creatorId);

	// Setting up default lobby name
	this->name.assign(creatorId);

	// Setting up default password
	this->password.assign("");
}

Game::Game(string mapName, string creatorId, int maxPlayers, string lobbyName)
{
	// Setting up game ID
	this->id = nextId;
	nextId++;

	// Loading up map
	this->map = Map::loadMap(mapName);

	// Initialization of lobby variables
	this->running = false;

	// Initialization of players
	this->activePlayer = -1;
	this->maxPlayers = min(maxPlayers, this->map.getMaxPlayers());
	this->nbPlayers = 0;
	addPlayer(creatorId);

	// Setting up lobby name
	this->name.assign(lobbyName);

	// Setting up default password
	this->password.assign("");
}

Game::Game(string mapName, string creatorId, int maxPlayers, string lobbyName, string password)
{
	// Setting up game ID
	this->id = nextId;
	nextId++;

	// Loading up map
	this->map = Map::loadMap(mapName);

	// Initialization of lobby variables
	this->running = false;

	// Initialization of players
	this->activePlayer = -1;
	this->maxPlayers = min(maxPlayers, this->map.getMaxPlayers());
	this->nbPlayers = 0;
	addPlayer(creatorId);

	// Setting up lobby name
	this->name.assign(lobbyName);

	// Setting up password
	this->password.assign(password);
}

// Allowed when !isRunning()
int Game::messageStart()
{
	// Might have to check if a certain player sent this message
	// Starting the game
	if (!running) start();
	else return -1;

	// Setting initial reinforcement for all players
	setInitialReinforcement();

	// Returning the player chosen for the first turn
	return activePlayer;
}

// Allowed when isRunning()
// Allowed in phase 0, 1, 2
int Game::messageEndPhase(int player)
{
	// Checking if the right player sent the message
	if (player != activePlayer) return -1;

	// Checks in phase 0
	if (phase == 0) {
		// The active player must have spent all of their reinforcement
		if (players[player].getReinforcement() > 0) return -2;

		// The active player must have less than 5 tokens
		if (players[player].countTokens() >= 5) return -3;
	}

	// Checks in phase 1
	if (phase == 1) {
		// An unfinished combat has to be resolved
		// combat.attackerId is always reset to -1 when no combat is taking place
		if (combat.attackerId != -1) return -4;
	}

	// Always allowed in phase 2

	// All checks have been performed, we can proceed to the next phase
	if (phase == 2) {
		nextPlayer();
	}

	// Returns 0 if ok or number of reinforcement when applicable
	if (nextPhase() == 0) return turnReinforcement;
	else return 0;
}

// Allowed in phase -1, 0
int Game::messagePut(int player, int territory, int units)
{
	// Checking if the right player sent the message
	if (player != activePlayer) return -1;

	// Treatment in phase -1
	if (phase == -1) {
		// Currently no check needed on player available reinforcement, since the game will forcefull proceed to next phase once they're all out
		// A player can only put one unit at a time in this phase
		if (units != 1) return -2;

		// If there are still free territories, they have to put their unit in one of them
		if (freeTerritories > 0) {
			if (board[territory].owner != -1) return -3;
			else {
				board[territory].owner = player;
				board[territory].units = 1;

				freeTerritories--;
				players[player].spendReinforcement(1);
				players[player].addTerritoriesOwned();
			}
		}

		// If there are no more free territories, they have to put their unit in one of theirs
		else if (freeTerritories == 0) {
			if (board[territory].owner != player) return -4;
			else {
				putUnits(territory, 1);
			}
		}

		// Unit successfully put, we can proceed to next player
		nextPlayer();

		// If all players are out of reinforcement, we can finally move to phase 0 and start the game
		size_t i;
		size_t max = players.size();
		int count = 0;
		for (i = 0; i < max; i++) {
			count += players[i].getReinforcement();
		}
		if (count == 0)
		{
			if (nextPhase() == 0) return turnReinforcement();
			else return 0;
		}
		return 0;
	}

	// Treatment in phase 0
	if (phase == 0) {
		return putUnits(territory, units);
	}

	return -1;
}

// Allowed in phase 0
int Game::messageUseTokens(int player, int token1, int token2, int token3)
{
	// Checking if the right player sent the message
	if (player != activePlayer) return -1;

	// Phase check
	if (phase != 0) return -2;

	// Set validity check
	if (!isValidSet(token1, token2, token3)) return -4;

	// Treatment
	return useSet(token1, token2, token3);
}

// Allowed in phase 1
int Game::messageAttack(int player, int source, int destination, int units)
{
	// Checking if the right player sent the message
	if (player != activePlayer)
	{
		cerr << "MSG_ATT: It's not Player" + to_string(player) +\
		"'s turn, exiting...";
		 return -1;
	}

	// Phase check
	if (phase != 1)
	{
		cerr << "MSG_ATT: Phase check failed, exiting..." << endl;
		return -2;
	}
	// Checking if units is a valid amount
	if (units < 1 || units > 3)
	{
		cerr << "MSG_ATT: Units check failed, exiting..." << endl;
		return -3;
	}
	// Checking if a combat is not currently taking place
	if (combat.attackerId != -1)
	{
		cerr << "MSG_ATT: Combat already taking place, exiting..."\
		<< endl;
	 	return -4;
	}
	// Checking if the player owns the source
	if (board[source].owner != player)
	{
		cerr << "MSG_ATT: Active player doesn't own the source, \
		exiting..." << endl;
		return -5;
	}
	// Checking if the players doesn't own the destination
	if (board[destination].owner == player)
	{
		cerr << "MSG_ATT: You can't attack your own territory, exiting..."\
		<< endl;
		return -6;
	}
	// Checking if the territories are adjacent
	if (!areAdjacent(source, destination))
	{
		cerr << "MSG_ATT: Territories aren't adjacent, exiting..." << endl;
		return -7;
	}
	// Checking if the player has the required units
	// <= since one unit must remain on the source
	if (board[source].units <= units)
	{
		cerr << "MSG_ATT: One unit must remain on the source, exiting..." << endl;
		return -8;
	}

	// All checks have been performed, the attack is thus allowed and waiting for the defender's response
	combat.attackerId = player;
	combat.defenderId = board[destination].owner;
	combat.source = source;
	combat.destination = destination;
	combat.attackerUnits = units;

	return 0;

}

// Check combatOutcome.outcomeType for return values
// Allowed in phase 1
CombatOutcome Game::messageDefend(int player, int units)
{
	CombatOutcome result;
	result.outcomeType = -1;
	result.attackerLoss = -1;
	result.defenderLoss = -1;
	result.source = -1;
	result.destination = -1;
	result.capToken = -1;

	// Phase check
	if (phase != 1)
	{
		cerr << "MSG_DEF: Phase check failed, exiting..." << endl;
		result.outcomeType = -3;
		return result;
	}

	// Checking if a combat requires solving
	if (combat.attackerId == -1)
	{
		cerr << "MSG_DEF: No combat requires solving, exiting..." << endl;
		result.outcomeType = -2;
		return result;
	}

	// Checking if the right player sent the message
	if (player != combat.defenderId)
	{
		cerr << "MSG_DEF: The player is not the right defender, exiting..." << endl;
		result.outcomeType = -1;
		return result;
	}

	// Checking if units is a valid amount
	if (units < 1 || units > 2){
		cerr << "MSG_DEF: Units check failed, exiting..." << endl;
		result.outcomeType = -4;
		return result;
	}
	// Checking if the player has the required units
	// < since the defender can use all of their units
	if (board[combat.destination].units < units)
	{
		cerr << "MSG_DEF: Not enough units on the territory, exiting..."\
		<< endl;
		result.outcomeType = -5;
		return result;
	}

	// All checks have been performed, the combat can now be solved
	combat.defenderUnits = units; // Unnecessary, but kept for consistency for now
	result = solveCombat(combat.attackerUnits, combat.defenderUnits);

	// Updating result
	result.outcomeType = 0;
	result.source = combat.source;
	result.destination = combat.destination;

	// Updating the unit count on both territories
	board[combat.source].units -= result.attackerLoss;
	board[combat.destination].units -= result.defenderLoss;

	// Updating turn variables
	lastAttackingTerritory = combat.source;
	lastAttackedTerritory = combat.destination;

	// Updating unitsLost and unitsKilled
	players[combat.attackerId].unitsKilled += result.defenderLoss;
	players[combat.defenderId].unitsKilled += result.attackerLoss;
	players[combat.attackerId].unitsLost += result.attackerLoss;
	players[combat.defenderId].unitsLost += result.defenderLoss;

	// Checking if the combat resulted in a capture
	if (board[combat.destination].units == 0) {
		// Check for granting a token to the attacker
		if (!territoryCapture) {
			result.capToken = grantToken();
			territoryCapture = true;
		}

		// Proceeding with the territory capture
		// In the case of a capture, the attacking player didn't lose any unit, so we can move them directly to the destination and update the owner
		board[combat.source].units -= combat.attackerUnits;
		board[combat.destination].units = combat.attackerUnits;
		board[combat.destination].owner = combat.attackerId;

		// Updating players involved
		players[combat.attackerId].addTerritoriesOwned();
		players[combat.defenderId].subTerritoriesOwned();
		players[combat.attackerId].addTerritoriesCaptured();
		players[combat.defenderId].addTerritoriesLost();

		// Checking for player elimination
		if (players[combat.defenderId].getTerritoriesOwned() == 0) {
			players[combat.defenderId].die();

			// Updating result
			result.outcomeType = 1;

			// Token transfer between defender and attacker
			// Temporary crude method
			result.tokens[0] = players[combat.defenderId].countTokensOfType(0);
			result.tokens[1] = players[combat.defenderId].countTokensOfType(1);
			result.tokens[2] = players[combat.defenderId].countTokensOfType(2);
			result.tokens[3] = players[combat.defenderId].countTokensOfType(3);

			players[combat.defenderId].receiveTokens(result.tokens);

			// Updating eliminationCount and checking for victory
			eliminationCount++;
			if (eliminationCount >= nbPlayers - 1) result.outcomeType = 2;
		}

		// Updating lastAttackCapture
		lastAttackCapture = true;
	}
	else lastAttackCapture = false;


	// Resetting the CombatHandler
	resetCombat();
	return result;
}

// Allowed in phase 1, 2
int Game::messageMove(int player, int source, int destination, int units)
{
	// Checking if the right player sent the message
	if (player != activePlayer) return -1;

	// Treatment in phase 1
	if (phase == 1) {
		// Checking if the last attack resulted in a capture
		if (!lastAttackCapture) return -3;
		// Checking if the territories are the ones involved in the last combat
		if (lastAttackingTerritory != source) return -4;
		if (lastAttackedTerritory != destination) return -4;

		// Proceeding with the move
		return moveUnits(source, destination, units);
	}

	// Treatment in phase 2
	if (phase == 2) {
		// Checking if the player already used his free move this turn
		if (!moved) {
			if (moveUnits(source, destination, units) == 0) {
				moved = true; // Putting this in moveUnits() would generate conflicts with phase 1 moves or require extra manual resets
				return 0;
			}
			else return -6;
		}
		else return -5;
	}

	return -2;

}


string Game::toJson()
{
	string res="{\"name\":\""+name+"\",\"nbPlayers\":"+to_string(nbPlayers)+",";
	res+= "\"players\":[";
				for(size_t i = 0;i<players.size();i++)
				{
					res+=players.at(i).toJson();
					if(i<players.size()-1)
						res+=",";
				}
			res+="],";
	res+="\"mapName\":\""+map.getName()+"\",";
	res+="\"board\":[";
				for(size_t i = 0; i<board.size();i++)
				{
					res+="{\"terId\":"+to_string(i)+",\"ownerId\":"+to_string(board.at(i).owner)+",\"nbUnits\":"+to_string(board.at(i).units)+"}";
					if(i<board.size()-1)
						res+=",";
				}
			res+="],";
	res+="\"freeTerritories\":"+to_string(freeTerritories)+",";
	res+="\"phase\":"+to_string(phase)+",";
	res+="\"activePlayer\":"+to_string(activePlayer)+",";
	res+="\"totalExchangedSets\":"+to_string(totalExchangedSets)+"}";

	return res;
}

CombatHandler Game::getCombat()
{
	return combat;
}
