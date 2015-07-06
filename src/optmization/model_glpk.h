#ifndef SYNTHESIZER_OPTIMIZATION_GLPK_MODEL_H_
#define SYNTHESIZER_OPTIMIZATION_GLPK_MODEL_H_

#include "glpk_lp.h"

using namespace std;
using namespace math;

namespace optimization
{

class GlpkModel : public GlpkLp
{
  public:
    GlpkModel(int rows, int cols);
    void add_drain();
    
};

} // namespace optimization 

#endif // SYNTHESIZER_OPTIMIZATION_GLPK_MODEL
