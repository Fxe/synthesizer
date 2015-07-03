#include "stoichiometric_model.h"

using namespace model;

StoichiometricModel::StoichiometricModel()
{
}

StoichiometricModel::StoichiometricModel(const int c, const int r, 
                                         double** S)
{
  this->rows_ = r;
  this->cols_ = c;
  int l = 0;
  for (int i = 0; i < r; i++)
  {
    for (int j = 0; j < c; j++)
    {
      S[i][j];
    }
  }
}