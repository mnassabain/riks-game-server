#ifndef MAP_H
#define MAP_H

#include <vector>
#include <iterator>
#include <fstream>
#include <sstream>
#include "Structs.h"
#include "utils.h"

using namespace std;
class Map {
private:
  //Attributes
  string name;
  int maxPlayers;
  vector <Continent> continents;
  vector <Territory> territories;
  static vector <Map> maps;

  //Constructors

  /**
   * @brief Map : Constructor
   * @param file : map name
   */
  Map(string name);

  //Methods

  /**
   * @brief exists : Checking method to see if a map already exists
   * @param name : name of the map
   * @return true if the map exists, false otherwise
   */
   static bool exists(string name);

  /**
   * @brief getMapIndex : get the index of a Map in the maps list
   * @param name : name of the map
   * @return index of the map if it exists, -1 otherwise
   */
   static int getMapIndex(string name);

public:

  //Constructor
  /**
   * @brief Map : Default constructor (debug map)
   */
  Map();

  //Methods

  /**
   * @brief getName : Getter on map's name
   * @return map name
   */
  string getName();

  /**
   * @brief getMaxPlayers : Getter on map's maximum number of players
   * @return map max players
   */
  int getMaxPlayers();

  /**
   * @brief nbTerritories : get the number of territories
   * @return Number of territories
   */
  int nbTerritories();

  /**
   * @brief nbContinents : get the number of continents
   * @return Number of continents
   */
  int nbContinents();

  /**
   * @brief getContinents : Getter on map's continents
   * @return continents
   */
  vector<Continent> getContinents();

  /**
   * @brief getContinentById : return a copy of the continent identified by the given id
   * @param id : id of the continent
   * @return continent copy (or a Continent with id == -1 if it doesn't exist)
   */
  Continent getContinentById(int id);

  /**
   * @brief getTerritories : Getter on map's territories
   * @return territories
   */
  vector<Territory> getTerritories();

  /**
   * @brief loadMap : load the reference of the map
   * @param name : name of the map to be loaded
   * @return Map reference (DebugMap if the map file doesn't exists)
   */
  static Map loadMap(string name);
};

#endif //MAP_H
