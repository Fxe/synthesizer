#include "reaction.h"

bio::Reaction::Reaction()
{
}

void bio::Reaction::set_reversible(bool reversible)
{
  this->reversible_ = reversible;
}

bool bio::Reaction::is_reversible()
{
  return this->reversible_;
}

void bio::Reaction::add_lhs_stoichiometry(int cpd_id, double value)
{
  pair<int, double> s(cpd_id, value);
  this->lhs_.push_back(s);
}

void bio::Reaction::add_rhs_stoichiometry(int cpd_id, double value)
{
  pair<int, double> s(cpd_id, value);
  this->rhs_.push_back(s);
}

vector<pair<int, double>> bio::Reaction::get_lhs_stoichiometry()
{
  return this->lhs_;
}

vector<pair<int, double>> bio::Reaction::get_rhs_stoichiometry()
{
  return this->rhs_;
}
