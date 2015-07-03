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

void bio::Reaction::add_stoichiometry(int cpd_id, double value)
{
  pair<int, double> s(cpd_id, value);
  this->lhs_.push_back(s);
}