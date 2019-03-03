#ifndef MAP_H
#define MAP_H

#include <list>
#include <iterator>
#include "Structs.h"

using namespace std;
class Map {
private:
  //Attributes
  string name;
  list <Continent> continents;
  list <Territory> territories;
  static list <Map> maps;

  //Constructors

  /**
   * @brief Map : Default constructor (debug map)
   */
  Map();

  /**
   * @brief Map : Constructor
   * @param file : path of the file used to create the map
   */
  Map(string file);

  //Methods

  /**
   * @brief exists : Checking method to see if a map already exists
   * @param name : name of the map
   * @return true if the map exists, false otherwise
   */
   static bool exists(string name);

public:

  //Methods

  /**
   * @brief getName : Getter on map's name
   * @return map name
   */
  string getName();

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
  list<Continent> getContinents();

  /**
   * @brief getTerritories : Getter on map's territories
   * @return territories
   */
  list<Territory> getTerritories();

  /**
   * @brief loadMap : load the reference of the map
   * @param name : name of the map to be loaded
   * @return Map reference
   */
  static Map loadMap(string name);
};
#endif
