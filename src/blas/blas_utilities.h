#ifndef SYNTHESIZER_BLAS_BLAS_UTILITIES_H_
#define SYNTHESIZER_BLAS_BLAS_UTILITIES_H_

#include <vector>

using namespace std;

namespace blas
{
  double WallTime();
  double CpuTime();
  double * dInitArray(const int s, const double value);
  double ** dInitMatrix(const int r, const int c, const double value);
  void read_matrix_file(const string path,
                        int* rows, int* cols,
                        double*** matrix, int** rev,
                        double** ub, double **lb,
                        vector<string>* rxn_alias,
                        vector<string>* cpd_alias);
  //int ** InitMatrix(int r, int c);
} //namespace blas

#endif // SYNTHESIZER_BLAS_BLAS_UTILITIES