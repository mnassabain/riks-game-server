/**
 * @file Structs.h
 * @brief 
 */

#ifndef STRUCTS_H
#define STRUCTS_H

#include <vector>
#include <string>
using namespace std;

//Territory
struct Territory
{
  int id;
  vector <int> neighbors;
};
typedef struct Territory Territory;

//Continent
struct Continent
{
  int id;
  int bonus;
  int firstTerritory;
  int lastTerritory;
};
typedef struct Continent Continent;

//TerritoryState
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

//CombatOutcome
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

//CombatHandler
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
