#include "stoichiometric_model.h"
#include <iostream>
#include <string>

using namespace std;
using namespace model;

StoichiometricModel::StoichiometricModel()
{
}

StoichiometricModel::StoichiometricModel(const int c, const int r, 
                                         double** S)
{
  this->matrix_ = new Matrix(r, c);
  int l = 0;
  for (int i = 0; i < r; i++)
  {
    for (int j = 0; j < c; j++)
    {
      this->matrix_->set_value(i, j, S[i][j]);
    }
  }
}

StoichiometricModel::StoichiometricModel(Matrix* matrix,
                                         vector<double>* lb,
                                         vector<double>* ub,
                                         vector<string>* cpd_alias,
                                         vector<string>* rxn_alias)
{
  this->matrix_ = new Matrix(*matrix);
  this->lb_ = new vector<double>(*lb);
  this->ub_ = new vector<double>(*ub);
  this->cpd_alias_ = new vector<string>(*cpd_alias);
  this->rxn_alias_ = new vector<string>(*rxn_alias);
}

Matrix* StoichiometricModel::get_matrix() const
{
  return this->matrix_;
}

pair<double, double> StoichiometricModel::get_rxn_bounds(int i) const
{
  pair<double, double> p = { (*lb_)[i], (*ub_)[i] };
  return p;
}

void StoichiometricModel::set_rxn_bounds(int i, double lb, double ub) const
{
  (*lb_)[i] = lb;
  (*ub_)[i] = ub;
}

string StoichiometricModel::get_cpd_alias(int i) const
{
  return (*cpd_alias_)[i];
}

string StoichiometricModel::get_rxn_alias(int i) const
{
  return (*rxn_alias_)[i];
}
