#include "glpk_lp.h"
#include <string>
#include <iostream>

using namespace std;
using namespace optimization;

GlpkLp::GlpkLp(int rows, int cols)
{
  cout << "Init LP prob" << endl;
  this->rows = rows;
  this->cols = cols;
  this->lp = glp_create_prob();
  int ne = rows * cols;
  this->ia = new int[ne + 1];
  this->ja = new int[ne + 1];
  this->ar = new double[ne + 1];

  glp_add_rows(this->lp, rows);
  glp_add_cols(this->lp, cols);
  
  int l = 0;
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      //glpk indexs starts from 1
      this->ia[l + 1] = i + 1;
      this->ja[l + 1] = j + 1;
      this->ar[l + 1] = 0;
      l++;
      //cout << "(" << i + 1 << ", " << j + 1 << ") " << 0 << "\t";
    }
    //cout << endl;
  }
};

GlpkLp::~GlpkLp()
{
  delete this->ia;
  delete this->ja;
  delete this->ar;
  glp_delete_prob(lp);
};

void GlpkLp::set_name(string name)
{
  glp_set_prob_name(this->lp, name.c_str());
};

void GlpkLp::set_row_alias(int row, string name)
{
  glp_set_row_name(this->lp, row, name.c_str());
};

void GlpkLp::set_col_alias(int col, string name)
{
  glp_set_row_name(this->lp, col, name.c_str());
};

void GlpkLp::set_obj_coef(int var, double coef)
{
  glp_set_obj_coef(this->lp, var, coef);
};

void GlpkLp::set_obj_dir(Op op)
{
  int dir = to_glp_bound_type(op);
  glp_set_obj_dir(this->lp, dir);
};

void GlpkLp::set_row_bound(int var, Op op, double value)
{
  int type = this->to_glp_bound_type(op);
  glp_set_row_bnds(this->lp, var, type, 0.0, value);
}

void GlpkLp::set_col_bound(double lb, Op op_lb, int var, Op op_ub, double ub)
{
  int type = this->to_glp_bound_type(op_lb);
  glp_set_col_bnds(this->lp, var, type, lb, ub);
}

void GlpkLp::set_matrix()
{

}

void GlpkLp::set_matrix_value(int row, int col, double value)
{
  if (row < this->rows && col < this->cols)
  {
    this->ar[cols * row + col] = value;
  }
  else
  {
    cerr << "error !" << endl;
  }
}

void GlpkLp::load_matrix()
{
  glp_set_col_bnds(this->lp, 1, GLP_LO, 0.0, 0.0);
  glp_set_col_bnds(this->lp, 1, GLP_LO, 0.0, 0.0);
  cout << "load matrix" << endl;
  int ne = this->rows * this->cols;
  for (int i = 0; i < ne; i++)
  {
    cout << "ia[" << i + 1 << "] = " << this->ia[i + 1] << endl;
    cout << "ja[" << i + 1 << "] = " << this->ja[i + 1] << endl;
    cout << "ar[" << i + 1 << "] = " << this->ar[i + 1] << endl;
  }
  glp_load_matrix(this->lp, ne, this->ia, this->ja, this->ar);
}

double GlpkLp::solve()
{
  glp_simplex(this->lp, NULL);
  double z = glp_get_obj_val(this->lp);
  return z;
}

int GlpkLp::to_glp_bound_type(Op op)
{
  int type = 0;

  switch (op)
  {
    case math::Op::EQUAL:
      type = GLP_FX;
      break;
    case math::Op::GE:

      break;
    case math::Op::LE:
      type = GLP_UP;
      break;
    case math::Op::MAX:
      type = GLP_MAX;
      break;
    case math::Op::MIN:
      type = GLP_MIN;
      break;
    default:
      type = GLP_UP;
      break;
  }

  return type;
}