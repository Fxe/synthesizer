#ifndef SYNTHESIZER_BLAS_BLAS_UTILITIES_H_
#define SYNTHESIZER_BLAS_BLAS_UTILITIES_H_

namespace blas
{
  double * dInitArray(const int s, const double value);
  double ** dInitMatrix(const int r, const int c, const double value);
  //int ** InitMatrix(int r, int c);
} //namespace blas

#endif // SYNTHESIZER_BLAS_BLAS_UTILITIES