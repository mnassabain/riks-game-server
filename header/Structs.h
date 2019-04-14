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
	int attackerLoss;
	int defenderLoss;
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
