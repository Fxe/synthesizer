#ifndef SYNTHESIZER_PGRAPH_SSG_H_
#define SYNTHESIZER_PGRAPH_SSG_H_

#include "pgraph.h"
#include <set>
#include <map>

using namespace std;

namespace pgraph
{

class SolutionStructureGeneration
{
  public:
    SolutionStructureGeneration(int p, set<int>* r, pgraph::ProcessGraph* g);
    void solve();
  private:
    ProcessGraph* pg_;
    set<int>* p_;
    set<int>* r_;
    map<int, set<int>*> deltaMap_;
    void ssg(set<int>* p, set<int>* m, map<int, set<int>>*);
    void initDeltaMap();
    set<int>* delta(int x);
    bool test(set<int> c, set<int>* m, map<int, set<int>> *d_map);
};

} // namespace pgraph

#endif // SYNTHESIZER_PGRAPH_SSG
