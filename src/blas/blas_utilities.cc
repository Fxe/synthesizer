#include "blas_utilities.h"

double* blas::dInitArray(const int s, const double value)
{
  double* v = new double[s];
  for (int i = 0; i < s; i++)
  {
    v[i] = value;
  }
  return v;
}

double** blas::dInitMatrix(const int r, const int c, const double value)
{
  double** m = new double*[r];
  // naive way ! non contiguos
  for (int i = 0; i < r; i++)
  {
    m[i] = dInitArray(c, value);
  }
  // better ! too lazy to implement
  //m[0] = new double[r * c];
  return m;
}