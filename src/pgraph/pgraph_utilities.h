#ifndef SYNTHESIZER_PGRAPH_PGRAPH_UTILITIES_H_
#define SYNTHESIZER_PGRAPH_PGRAPH_UTILITIES_H_

#include <map>
#include <set>

using namespace std;

namespace pgraph
{

  set<set<int>> PowerSet(set<int> s);
  string set_string(set<int> s);
  string map_string(map<int, set<int>> m);
} // namespace pgraph 

#endif // SYNTHESIZER_PGRAPH_PGRAPH_UTILITIES
