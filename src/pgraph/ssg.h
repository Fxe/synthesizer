#ifndef SYNTHESIZER_PGRAPH_SSG_H_
#define SYNTHESIZER_PGRAPH_SSG_H_

#include "pgraph.h"

namespace pgraph
{

class SolutionStructureGeneration
{
  public:
    SolutionStructureGeneration(pgraph::ProcessGraph* g);
    void solve();
  private:
};

}
#endif // SYNTHESIZER_PGRAPH_SSG
