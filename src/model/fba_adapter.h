#ifndef SYNTHESIZER_MODEL_FBA_ADAPTER_H_
#define SYNTHESIZER_MODEL_FBA_ADAPTER_H_

#include <vector>
#include <map>

#include "optmization/glpk_lp.h"
#include "stoichiometric_model.h"

namespace model
{

class FluxBalanceSolution
{
public:
  FluxBalanceSolution(int size);
  ~FluxBalanceSolution();
  double obj;
  double flux_sum();
  vector<double>* flux_vector;

};

//rename to StoichiometricMatrix
class FluxBalanceAnalysisAdapter
{
  public:
    FluxBalanceAnalysisAdapter(const StoichiometricModel& sm);
    void knock_out_reaction(int rxnIndex);
    void knock_out_reaction(string rxnId);
    void reset_knockouts();
    double solve();
    void solve2();

    FluxBalanceSolution* fba();
    FluxBalanceSolution* pfba(int obj);

    optimization::GlpkLp* get_lp() const;
  private:
    vector<pair<double, double>>* default_bounds_;
    //what the hell ?
    vector<int>* dirty_bounds_;
    StoichiometricModel* sm_;
    //this should be a linear program interface
    optimization::GlpkLp* lp_;
};

} //namespace model

#endif // SYNTHESIZER_MODEL_FBA_ADAPTER