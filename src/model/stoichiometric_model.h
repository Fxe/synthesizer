#ifndef SYNTHESIZER_MODEL_STOICHIOMETRIC_MODEL_H_
#define SYNTHESIZER_MODEL_STOICHIOMETRIC_MODEL_H_
#include <vector>

using std::vector;
using std::pair;

namespace model
{

class StoichiometricModel
{
  public:
    StoichiometricModel();
    StoichiometricModel(const int c, const int r, double** S);
    void get_reaction();
    void get_specie();
  private:
    int rows_;
    int cols_;
    double **matrix_;
    bool *rev_;
};

} //namespace bio

#endif // SYNTHESIZER_MODEL_STOICHIOMETRIC_MODEL_H_