#include "matrix.h"

using namespace blas;
Matrix::Matrix(int rows, int cols)
{
  this->rows_ = rows;
  this->cols_ = cols;
  this->data_ = new double*[rows];
  //data[0] = new double[rows * cols];
  for (int i = 0; i < rows; i++)
  {
    this->data_[i] = new double[cols];
    for (int j = 0; j < cols; j++)
    {
      this->data_[i][j] = 0;
    }
  }
}

Matrix::Matrix(const Matrix &matrix)
{
  rows_ = matrix.get_rows();
  cols_ = matrix.get_cols();
  data_ = new double*[rows_];
  for (int i = 0; i < rows_; i++)
  {
    data_[i] = new double[cols_];
    for (int j = 0; j < cols_; j++)
    {
      data_[i][j] = matrix.get(i, j);
    }
  }
}

Matrix::~Matrix()
{
  delete this->data_;
}

void Matrix::set_value(int row, int col, double value) const
{
  this->data_[row][col] = value;
}

int Matrix::get_cols() const
{
  return this->cols_;
}

int Matrix::get_rows() const
{
  return this->rows_;
}

int Matrix::size() const
{
  return this->rows_ * this->cols_;
}

double Matrix::get(int row, int col) const
{
  return this->data_[row][col];
}