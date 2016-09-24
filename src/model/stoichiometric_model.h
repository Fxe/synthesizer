#ifndef SYNTHESIZER_MODEL_STOICHIOMETRIC_MODEL_H_
#define SYNTHESIZER_MODEL_STOICHIOMETRIC_MODEL_H_
#include <vector>
#include <string>
#include "blas/matrix.h"

using namespace std;
using std::vector;
using std::pair;

using blas::Matrix;

namespace model
{

//rename to StoichiometricMatrix
class StoichiometricModel
{
  public:
    StoichiometricModel();
    StoichiometricModel(const int c, const int r, double** S);
    StoichiometricModel(Matrix* matrix,
                        vector<double>* lb, vector<double>* ub,
                        vector<string>* cpd_alias, vector<string>* rxn_alias);

    Matrix* get_matrix() const;
    pair<double, double> get_rxn_bounds(int i) const;
    void set_rxn_bounds(int i, double lb, double ub) const;
    void get_reaction();
    void get_specie();

    //aliases
    void set_cpd_alias(int i, string alias);
    void set_rxn_alias(int i, string alias);
    
    string get_cpd_alias(int i) const;
    string get_rxn_alias(int i) const;
  private:
    Matrix* matrix_;
    vector<double>* lb_;
    vector<double>* ub_;
    vector<string>* cpd_alias_;
    vector<string>* rxn_alias_;
};

} //namespace model

#endif // SYNTHESIZER_MODEL_STOICHIOMETRIC_MODEL
