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

#endif //STRUCTS_H
