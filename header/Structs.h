/**
 * @file Structs.h
 * @brief Holds the structs needed
 */

#ifndef STRUCTS_H
#define STRUCTS_H

#include <vector>
#include <string>
using namespace std;

/**
 * @brief Structure representing a territory in the map
 * @struct Territory Structs.h "Territory"
 * 
 */
struct Territory
{
  int id;
  vector <int> neighbors;
};
typedef struct Territory Territory;

/**
 * @brief Structure representing a continent in the map
 * @struct Continent Structs.h "Continent"
 * 
 */
struct Continent
{
  int id;
  int bonus;
  int firstTerritory;
  int lastTerritory;
};
typedef struct Continent Continent;

/**
 * @brief Structure representing the state of a territory in the map
 * @struct TerritoryState Structs.h "TerritoryState"
 * 
 */
struct TerritoryState
{
  string toString()
  {
    string status;
    status = "owner = " + to_string(owner) + ", units = "\
    + to_string(units);
    return status;
  }
  int owner;
  int units;
};
typedef struct TerritoryState TerritoryState;

/**
 * @brief Structure representing the outcome of a combat
 * @struct CombatOutcome Structs.h "CombatOutcome"
 * 
 */
struct CombatOutcome
{
	// Outcome Type
	int outcomeType;

	// Unit loss
	int attackerLoss;
	int defenderLoss;

	// Territories involved
	int source;
	int destination;

   // Token granted in the case of the first capture of a turn (-1 if not applicable)
   int capToken;

	// Tokens transferred in the case of a player elimination
	int tokens[4];
};
typedef struct CombatOutcome CombatOutcome;

/**
 * @brief Structure representing the combat handler
 * @struct CombatHandler Structs.h "CombatHandler"
 * 
 */
struct CombatHandler
{
	int attackerId;
	int defenderId;
	int source;
	int destination;
	int attackerUnits;
	int defenderUnits;
};
typedef struct CombatHandler CombatHandler;

#endif //STRUCTS_H
