#ifndef SYNTHESIZER_BLAS_MATRIX_H_
#define SYNTHESIZER_BLAS_MATRIX_H_

#include <vector>

using namespace std;

namespace blas
{
class Matrix
{
  public:
    Matrix(int rows, int cols);
    Matrix(const Matrix &matrix);
    ~Matrix();
    int get_rows() const;
    int get_cols() const;
    double get(int row, int col) const;
    int size() const;
    void set_value(int row, int col, double value) const;
  private:
    int rows_;
    int cols_;
    double** data_;
};
} //namespace blas

#endif // SYNTHESIZER_BLAS_MATRIX