#include "../header/Map.h"
#include <iostream>
using namespace std;

vector<Map> Map::maps = vector<Map>();

Map::Map()
{
  this->name="DebugMap";
  this->maxPlayers=6;
  this->continents=vector<Continent>();
  this->territories=vector<Territory>();

  Continent c1;
  c1.id=0;
  c1.bonus=1;
  c1.firstTerritory=0;
  c1.lastTerritory=2;

  Continent c2;
  c2.id=1;
  c2.bonus=2;
  c2.firstTerritory=3;
  c2.lastTerritory=5;

  continents.push_back(c1);
  continents.push_back(c2);

  for(int i =0;i<6;i++)
  {
    Territory t;
    t.id=i;
    territories.push_back(t);
  }
}

Map::Map(string file)
{

}

Map Map::loadMap(string name)
{
  Map m;
  int tmp;
  if ((tmp = Map::getMapIndex(name)) != -1)
  { 
    m = maps.at(tmp);
    cout<<">> "<<name<<" map has been demanded"<<endl;
  }
  else if(name == "DebugMap")
  {
    cout<<">> Loading DebugMap..."<<endl;
    m = Map();
    maps.push_back(m);
  }
  return m;
}

string Map::getName()
{
  return this->name;
}

int Map::nbTerritories()
{
  return this->territories.size();
}

int Map::nbContinents()
{
  return this->continents.size();
}


vector<Continent> Map::getContinents()
{
  return vector<Continent>(this->continents);
}

vector<Territory> Map::getTerritories()
{
  return vector<Territory>(this->territories);
}

bool Map::exists(string name)
{
  bool res = false;
  for(size_t i = 0;!res && i<Map::maps.size();i++)
  {
    if(Map::maps.at(i).getName()==name)
      res = true;
  }
  return res;
}

int Map::getMapIndex(string name)
{
  int res = -1;
  for(size_t i = 0; res == -1 && i<Map::maps.size();i++)
  {
    if(Map::maps.at(i).getName()==name)
      res=i;
  }
  return res;
}
