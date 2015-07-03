#ifndef SYNTHESIZER_BIO_REACTION_H_
#define SYNTHESIZER_BIO_REACTION_H_
#include <vector>

using std::vector;
using std::pair;

namespace bio
{

class Reaction
{
  public:
    Reaction();
    void set_reversible(bool reversible);
    bool is_reversible();
    void add_stoichiometry(int cpd_id, double value);
  private:
    int id_;
    bool reversible_;
    vector<pair<int, double>> lhs_;
    vector<pair<int, double>> rhs_;
};

} //namespace bio

#endif // SYNTHESIZER_BIO_REACTION