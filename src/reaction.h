#ifndef SYNTHESIZER_BIO_REACTION_H_
#define SYNTHESIZER_BIO_REACTION_H_
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using std::vector;
using std::pair;

namespace bio
{

class Reaction
{
  public:
    Reaction();
    Reaction(long id, string entry);
    void set_reversible(bool reversible);
    bool is_reversible();
    void add_lhs_stoichiometry(long cpd_id, double value);
    void add_rhs_stoichiometry(long cpd_id, double value);
    vector<pair<long, double>> get_lhs_stoichiometry();
    vector<pair<long, double>> get_rhs_stoichiometry();

    friend ostream& operator<<(ostream& os, const Reaction& rxn)
    {
      os << "rxn[{";
      
      for (int i = 0; i < rxn.lhs_.size(); i++)
      {
        pair<long, double> p = rxn.lhs_[i];
        os << "(" << p.first << ", " << p.second << ")";
      }
      if (rxn.reversible_)
      {
        os << "} <=> {";
      }
      else
      {
        os << "}  => {";
      }
      
      for (int i = 0; i < rxn.rhs_.size(); i++)
      {
        pair<long, double> p = rxn.rhs_[i];
        os << "(" << p.first << ", " << p.second << ")";
      }

      os << "}]";
      return os;
    }
    //friend ostream& operator<< (ostream& stream, const Reaction& rxn);
  private:
    long id_;
    string entry_;
    bool reversible_;
    vector<pair<long, double>> lhs_;
    vector<pair<long, double>> rhs_;
};

} //namespace bio

#endif // SYNTHESIZER_BIO_REACTION