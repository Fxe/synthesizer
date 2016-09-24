#include "glpk_lp.h"
#include <string>
#include <iostream>

using namespace std;
using namespace optimization;

GlpkLp::GlpkLp(int rows, int cols)
{
  cout << "GlpkLp::init(int, int) - Init LP prob" << endl;
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

  /*
  map<string, int> row_alias_map;
  map<string, int> col_alias_map;
  *row_alias_map_ = &row_alias_map;
  *col_alias_map_ = &col_alias_map;
  */
};

GlpkLp::~GlpkLp()
{
  delete this->ia;
  delete this->ja;
  delete this->ar;
  glp_delete_prob(lp);

  cout << "Dest LP prob" << endl;
};

int GlpkLp::get_rows() const
{
  return rows;
}

int GlpkLp::get_cols() const
{
  return cols;
}

void GlpkLp::set_name(string name)
{
  glp_set_prob_name(this->lp, name.c_str());
};

void GlpkLp::set_row_alias(int row, const string name)
{
  glp_set_row_name(this->lp, row + 1, name.c_str());
  (*row_alias_map_)[name] = row;
};

void GlpkLp::set_col_alias(int col, const string name)
{
  //cout << "Col: " << col + 1 << " -> " << name.c_str() << endl;
  glp_set_col_name(this->lp, col + 1, name.c_str());
  (*col_alias_map_)[name] = col;
};

int GlpkLp::get_row_index(const string alias) const
{
  
  return (*row_alias_map_)[alias];
}

int GlpkLp::get_col_index(const string alias) const
{
  return (*col_alias_map_)[alias];
}

void GlpkLp::set_obj_coef(int var, double coef)
{
  glp_set_obj_coef(this->lp, var + 1, coef);
};

void GlpkLp::set_obj_dir(Op op)
{
  int dir = to_glp_bound_type(op);
  glp_set_obj_dir(this->lp, dir);
};

void GlpkLp::set_row_bound(int var, Op op, double value)
{
  int type = this->to_glp_bound_type(op);
  glp_set_row_bnds(this->lp, var + 1, type, 0.0, value);
}

void GlpkLp::set_col_bound(double lb, Op op_lb, int var, Op op_ub, double ub)
{
  int type = this->to_glp_bound_type(op_lb);
  if (op_lb == Op::UNBOUNDED && op_ub == Op::UNBOUNDED)
  {
    type = GLP_FR;
  }
  else if (op_lb == Op::UNBOUNDED)
  {
    type = GLP_UP;
  }
  else if (op_ub == Op::UNBOUNDED)
  {
    type = GLP_LO;
  }
  else
  {
    type = GLP_DB;
    if (lb == ub){
      type = GLP_FX;
    }
  }
  //cout << var + 1 << " : " << type << endl;
  glp_set_col_bnds(this->lp, var + 1, type, lb, ub);
}

void GlpkLp::set_matrix(Matrix *matrix)
{
  const int rows = matrix->get_rows();
  const int cols = matrix->get_cols();
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      this->set_matrix_value(i, j, matrix->get(i, j));
    }
  }
}

void GlpkLp::set_matrix_value(int row, int col, double value)
{
  if (row < this->rows && col < this->cols)
  {
    this->ar[cols * row + col + 1] = value;
  }
  else
  {
    cerr << "error !" << endl;
  }
}

void GlpkLp::load_matrix()
{
  //glp_set_col_bnds(this->lp, 1, GLP_LO, 0.0, 0.0);
  //glp_set_col_bnds(this->lp, 2, GLP_LO, 0.0, 0.0);
  int ne = this->rows * this->cols;

  /*
  cout << "load matrix" << endl;
  
  for (int i = 0; i < ne; i++)
  {
    cout << "ia[" << i + 1 << "] = " << this->ia[i + 1] << endl;
    cout << "ja[" << i + 1 << "] = " << this->ja[i + 1] << endl;
    cout << "ar[" << i + 1 << "] = " << this->ar[i + 1] << endl;
  }
  */
  glp_load_matrix(this->lp, ne, this->ia, this->ja, this->ar);
}

double GlpkLp::solve()
{
  glp_smcp param;
  glp_init_smcp(&param);
  //param.tol_bnd = 1e-7; 
  glp_simplex(this->lp, &param);
  double z = glp_get_obj_val(this->lp);
  return z;
}

double GlpkLp::get_col_prim(int var)
{
  return glp_get_col_prim(this->lp, var + 1);
}

glp_prob* GlpkLp::get_lp()
{
  return this->lp;
}

int GlpkLp::write_lp(string path)
{
  return glp_write_lp(lp, NULL, path.c_str());
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

void GlpkLp::after_solve()
{
  for (int i = 0; i < cols; i++)
  {
    double pv = glp_get_col_prim(this->lp, i + 1);
    double dv = glp_get_col_dual(this->lp, i + 1);
    int stat = glp_get_col_stat(this->lp, i + 1);
    //glp_print
    //GLP_BS 1
    //GLP_NL 2
    //GLP_NU 3
    //GLP_NF 4
    //GLP_NS 5
    cout << glp_get_col_name(this->lp, i + 1) << " " << pv << " " << dv << " " << stat << endl;
    //_glp_lpx_print_sol(this->lp, "wut");
  }
  
}