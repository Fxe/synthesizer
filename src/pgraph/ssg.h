#ifndef SYNTHESIZER_PGRAPH_SSG_H_
#define SYNTHESIZER_PGRAPH_SSG_H_

#include "pgraph.h"
#include <set>

using namespace std;

namespace pgraph
{

class SolutionStructureGeneration
{

  public:
    SolutionStructureGeneration(int p, set<int>* r, pgraph::ProcessGraph* g);
    void solve();
  private:
    set<int>* p_;
    set<int>* r_;
    void ssg(set<int>* p, set<int>* m);
    void delta(int x);
    bool test(int c, set<int>* m, int somedelta);
};

}
#endif // SYNTHESIZER_PGRAPH_SSG
