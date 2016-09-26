#include "reaction.h"

bio::Metabolite::Metabolite()
{

}

bio::Metabolite::Metabolite(long id, string entry)
{
  this->id = id;
  this->entry = entry;
}

bio::Reaction::Reaction()
{
}

bio::Reaction::Reaction(long id, string entry)
{
  this->id = id;
  this->entry = entry;
}

void bio::Reaction::set_reversible(bool reversible)
{
  this->reversible_ = reversible;
}

bool bio::Reaction::is_reversible()
{
  return this->reversible_;
}

void bio::Reaction::set_stoichiometry(map<long, double> s)
{
  for (auto p : s)
  {
    if (p.second < 0.0)
    {
      this->add_lhs_stoichiometry(p.first, p.second);
    }
    else if (p.second > 0.0)
    {
      this->add_rhs_stoichiometry(p.first, p.second);
    }
  }
}

void bio::Reaction::add_lhs_stoichiometry(long cpd_id, double value)
{
  pair<int, double> s(cpd_id, value);
  this->lhs_.push_back(s);
}

void bio::Reaction::add_rhs_stoichiometry(long cpd_id, double value)
{
  pair<int, double> s(cpd_id, value);
  this->rhs_.push_back(s);
}

vector<pair<long, double>> bio::Reaction::get_lhs_stoichiometry()
{
  return this->lhs_;
}

vector<pair<long, double>> bio::Reaction::get_rhs_stoichiometry()
{
  return this->rhs_;
}
