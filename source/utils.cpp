#include "../header/utils.h"

vector<string> split (string s, char delim)
{
  vector<string> res;
  stringstream ss (s);
  string item;

  while(getline(ss,item,delim))
  {
    res.push_back(item);
  }

  return res;
}
