#ifndef STRUCTS_H
#define STRUCTS_H

#include <list>
using namespace std;

//Territory
struct Territory
{
  int id;
  list <int> neighbors;
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

#endif //STRUCTS_H
