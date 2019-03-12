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

Map::Map(string name)
{
  this->name = name;
}

Map Map::loadMap(string name)
{
  Map m;
  int tmp;

  //If the map is already loaded
  if ((tmp = Map::getMapIndex(name)) != -1)
  {
    m = maps.at(tmp);
    cout<<"Map >> "<<name<<" has been demanded"<<endl;
  }
  //Debug Map
  else if(name == "DebugMap")
  {
    cout<<"Map >> Loading DebugMap..."<<endl;
    m = Map();
    maps.push_back(m);
  }
  //Loading Map
  else
  {
    cout<<"Map >> Loading "<<name<<"..."<<endl;
    ifstream mapFile("maps/"+name+".map", ios::in);
    if(!mapFile)
    {
      cout<<"Map >> Cannont open file "<<name<<".map ! Returning DebugMap"<<endl;
      m=Map();
    }
    else
    {
      string line;
      int maxPlayers, nbContinents, nbTerritories;

      //Position of the line read (for map file error messages)
      int nline=0;

      //Reading map name and creating the object
      getline(mapFile,line);
      nline++;
      m = Map(line);

      //Reading max players
      getline(mapFile,line);
      nline++;
      maxPlayers= stoi(line);
      m.maxPlayers=maxPlayers;

      //Reading number of continents
      getline(mapFile,line);
      nline++;
      nbContinents= stoi(line);

      //Reading number of territories
      getline(mapFile,line);
      nline++;
      nbTerritories=stoi(line);

      //Empty Line
      getline(mapFile,line);
      nline++;

      //Reading continents
      int nbc=0;
      while(nbc <nbContinents && getline(mapFile,line))
      {
        nline++;
        vector<string> contInfos = split(line,' ');

        //Checking if line was empty
        if(contInfos.size() == 0)
        {
          cout<<"Map >> Error in "<<name<<".map (line: "<<nline<<") ! Declared "<<nbContinents<<" continents but only found "<<nbc<<". Returning DebugMap"<<endl;
          m=Map();
          mapFile.close();
          return m;
        }
        //Quickly checking line syntax
        else if(contInfos.size() != 4)
        {
          cout<<"Map >> Syntax error in file "<<name<<".map ! Bad continent description (line: "<<nline<<"). Returning DebugMap"<<endl;
          m=Map();
          mapFile.close();
          return m;
        }

        Continent c;
        c.id=stoi(contInfos.at(0)); //Id
        c.bonus=stoi(contInfos.at(1)); //Bonus
        c.firstTerritory=stoi(contInfos.at(2)); //FirstTerriroty
        c.lastTerritory=stoi(contInfos.at(3)); //LastTerritory

        //Adding continent to continents list
        m.continents.push_back(c);
        nbc++;
      }

      //Empty Line
      getline(mapFile,line);
      nline++;

      //Reading territories
      int nbt=0;
      while(nbt <nbTerritories && getline(mapFile,line))
      {
        nline++;
        vector<string> terInfos = split(line,' ');

        //Checking if line was empty
        if(terInfos.size() == 0)
        {
          cout<<"Map >> Error in "<<name<<".map (line: "<<nline<<") ! Declared "<<nbTerritories<<" territories but only found "<<nbt<<". Returning DebugMap"<<endl;
          m=Map();
          mapFile.close();
          return m;
        }

        Territory t;
        t.id=stoi(terInfos.at(0));
        //Neighbors
        for(size_t i = 1;i<terInfos.size();i++)
        {
          t.neighbors.push_back(stoi(terInfos.at(i)));
        }

        m.territories.push_back(t);

        nbt++;
      }

      //Final checking for the number of continents and territories (in case of incomplete file)
      if(nbt < nbTerritories || nbc < nbContinents)
      {
        cout<<"Map >> Error in "<<name<<".map ! Incomplete file (reached end before finding all continents or territories declared). Returning DebugMap"<<endl;
        m=Map();
        mapFile.close();
        return m;
      }

      mapFile.close();
      maps.push_back(m);
    }
  }
  return m;
}

string Map::getName()
{
  return this->name;
}

int Map::getMaxPlayers()
{
  return this->maxPlayers;
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
