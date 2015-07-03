/* short.c */
#include <iostream>
#include <vector>
#include <stdio.h>            /* C input/output                       */
#include <stdlib.h>           /* C standard library                   */
#include <glpk.h>             /* GNU GLPK linear/mixed integer solver */
#include "optmization/glpk_lp.h"
#include "reaction.h"
#include "blas/blas_utilities.h"

using namespace std;

//missing: obj coef
//bounds
glp_prob* foo(int r, int c, double** m,
              double* obj_coef,
              double* r_bd, int* r_bd_type,
              vector<string> r_alias, vector<string> c_alias)
{
  glp_prob *lp = glp_create_prob();
  glp_add_rows(lp, r);
  glp_add_cols(lp, c);
  glp_set_obj_dir(lp, GLP_MAX);
  int ne = r * c;
  int* ia = new int[ne + 1];
  int* ja = new int[ne + 1];
  double* ar = new double[ne + 1];
  int l = 0;
  for (int i = 0; i < r; i++)
  {
    for (int j = 0; j < c; j++)
    {
      //glpk indexs starts from 1
      ia[l + 1] = i + 1;
      ja[l + 1] = j + 1;
      ar[l + 1] = m[i][j];
      l++;
      cout << "(" << i + 1 << ", " << j + 1 << ") " << m[i][j] << "\t";
    }
    cout << endl;
  }
  for (int i = 0; i < r_alias.size(); i++)
  {
    string alias = r_alias[i];
    glp_set_row_name(lp, i + 1, alias.c_str());
  }
  for (int i = 0; i < c_alias.size(); i++)
  {
    string alias = c_alias[i];
    glp_set_col_name(lp, i + 1, alias.c_str());
  }
  for (int i = 0; i < c; i++)
  {
    glp_set_obj_coef(lp, i + 1, obj_coef[i]);
    glp_set_col_bnds(lp, i + 1, GLP_LO, -1.0, 1.0);
  }
  for (int i = 0; i < r; i++)
  {
    glp_set_row_bnds(lp, i + 1, r_bd_type[i], 0.0, r_bd[i]);
  }
  glp_load_matrix(lp, ne, ia, ja, ar);

  
  //glp_set_row_bnds(lp, 2, GLP_FX, 0.0, 0.0);
  glp_simplex(lp, NULL);
  return lp;
}

double demo2(int r, int c, double** m,
  double* o,
  double* r_bd, int* r_bd_type)
{
  vector<string> r_alias;
  vector<string> c_alias;
  r_alias.push_back("p");
  r_alias.push_back("q");
  c_alias.push_back("x1");
  c_alias.push_back("x2");
  glp_prob *lp = foo(r, c, m, o, r_bd, r_bd_type, r_alias, c_alias);
  glp_simplex(lp, NULL);
  double z = glp_get_obj_val(lp);
  glp_delete_prob(lp);
  glp_free_env();
  return z;
}

double demo3(int r, int c, double** m,
             double* o,
             double* r_bd, int* r_bd_type)
{
  const string c_names[] = { "R1", "R2", "R3", "R4", "R5", 
                             "R6", "R7", "R8", "R9", "R10" };
  const string r_names[] = { "A", "B", "C", "D", "E", "F" };
  vector<string> r_alias;
  vector<string> c_alias;
  for (int i = 0; i < c; i++)
  {
    c_alias.push_back(c_names[i]);
  }
  for (int i = 0; i < r; i++)
  {
    r_alias.push_back(r_names[i]);
  }
  glp_prob *lp = foo(r, c, m, o, r_bd, r_bd_type, r_alias, c_alias);
  glp_delete_prob(lp);
  glp_free_env();
  return 0;
}

/*
void metodo(const double m[static 2][static 2])
{

}
*/

double demo1()
{
  optimization::GlpkLp* lp = new optimization::GlpkLp(2, 2);
  lp->set_name("short");
  lp->set_obj_dir(Op::MAX);
  lp->set_row_alias(1, "p");
  lp->set_row_bound(1, Op::LE, 1.0); // <= 1.0
  lp->set_row_alias(2, "q");
  lp->set_row_bound(2, Op::LE, 2.0); // <= 2.0

  lp->set_col_alias(1, "x1");
  lp->set_obj_coef(1, 0.6);
  lp->set_col_alias(2, "x2");
  lp->set_obj_coef(2, 0.5);

  const double matrix[2][2] = {
      { 1.0, 2.0 },
      { 3.0, 1.0 },
  };

  lp->set_matrix_value(0, 0, 1.0); lp->set_matrix_value(0, 1, 2.0);
  lp->set_matrix_value(1, 0, 3.0); lp->set_matrix_value(1, 1, 1.0);
  lp->load_matrix();
  double opt = lp->solve();

  delete lp;
  return opt;
}

double demo0()
{
  /* declare variables */
  glp_prob *lp;
  int ia[1 + 1000], ja[1 + 1000];
  double ar[1 + 1000], z, x1, x2;
  /* create problem */
  lp = glp_create_prob();
  glp_set_prob_name(lp, "short");
  glp_set_obj_dir(lp, GLP_MAX);
  /* fill problem */
  glp_add_rows(lp, 2);
  glp_set_row_name(lp, 1, "p");
  glp_set_row_bnds(lp, 1, GLP_UP, 0.0, 1.0);
  glp_set_row_name(lp, 2, "q");
  glp_set_row_bnds(lp, 2, GLP_UP, 0.0, 2.0);
  glp_add_cols(lp, 2);
  glp_set_col_name(lp, 1, "x1");
  glp_set_col_bnds(lp, 1, GLP_LO, 0.0, 0.0);
  glp_set_obj_coef(lp, 1, 0.6);
  glp_set_col_name(lp, 2, "x2");
  glp_set_col_bnds(lp, 2, GLP_LO, 0.0, 0.0);
  glp_set_obj_coef(lp, 2, 0.5);
  ia[1] = 1, ja[1] = 1, ar[1] = 1.0; /* a[1,1] = 1 */
  ia[2] = 1, ja[2] = 2, ar[2] = 2.0; /* a[1,2] = 2 */
  ia[3] = 2, ja[3] = 1, ar[3] = 3.0; /* a[2,1] = 3 */
  ia[4] = 2, ja[4] = 2, ar[4] = 1.0; /* a[2,2] = 1 */
  glp_load_matrix(lp, 4, ia, ja, ar);
  /* solve problem */
  glp_simplex(lp, NULL);
  /* recover and display results */
  z = glp_get_obj_val(lp);
  x1 = glp_get_col_prim(lp, 1);
  x2 = glp_get_col_prim(lp, 2);
  //printf("z = %g; x1 = %g; x2 = %g\n", z, x1, x2);
  /* housekeeping */
  glp_delete_prob(lp);
  glp_free_env();
  return z;
}

void matrix(int r, int c, const double m[][2])
{
  for (int i = 0; i < r; i++)
  {
    for (int j = 0; j < c; j++)
    {
      cout << m[i][j] << " ";
    }
    cout << endl;
  }
}

/*
 * cols -> reactions
 * rows -> metabolites
 */
int main(int argc, char** argv)
{
  vector<int> array_int;
  vector<bio::Reaction> array_reaction;
  bio::Reaction r1;
  bio::Reaction r2;
  r2.set_reversible(true);
  cout << r1.is_reversible() << " " << r2.is_reversible() << endl;

  array_int.push_back(1);
  array_reaction.push_back(r1);
  
  const double m1[2][2] = {
      {1.0, 2.0},
      {3.0, 1.0},
  };
  const double m2[3][2] = {
      { 1.0, 2.0 },
      { 3.0, 1.0 },
      { 3.0, 5.0 },
  };
  const double m3[3][3] = {
      { 1.0, 2.0, 3.0 },
      { 3.0, 1.0, 3.0 },
      { 3.0, 5.0, 1.0 },
  };
  
  const char a = 'A';
  //const char **p = 0;
  //*p = &a;
  matrix(3, 3, m2);
 
  const int r = 2;
  const int c = 2;
  double** m = blas::dInitMatrix(r, c, 0.0);
  for (int i = 0; i < r; i++)
  {
    for (int j = 0; j < c; j++)
    {
      m[i][j] = m1[i][j];
    }
  }

  double const M[6][10] = {
      { 1, 0, 0, 0, -1, -1, -1, 0, 0, 0 }, //A
      { 0, 1, 0, 0, 1, 0, 0, -1, -1, 0 }, //B
      { 0, 0, 0, 0, 0, 1, 0, 1, 0, -1 }, //C
      { 0, 0, 0, 0, 0, 0, 1, 0, 0, -1 }, //D
      { 0, 0, 0, -1, 0, 0, 0, 0, 0, 1 }, //E
      { 0, 0, -1, 0, 0, 0, 0, 0, 1, 1 }, //F
  };

  double** M2 = blas::dInitMatrix(6, 10, 0.0);
  for (int i = 0; i < 6; i++)
  {
    for (int j = 0; j < 10; j++)
    {
      M2[i][j] = M[i][j];
    }
  }
  //double o_2[] = { 0.6, 0.5 };
  double o_2[] = { 1, 1 };
  double r_db_2[] = { 1.0, 2.0 };
  int r_db_type_2[] = { GLP_UP, GLP_UP };
  double o_3[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
  double r_db_3[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  int r_db_type_3[] = { GLP_FX, GLP_FX, GLP_FX, GLP_FX, GLP_FX, GLP_FX };
  cout << "demo0: " << demo0() << endl;
  cout << "demo0: " << demo0() << endl;
  cout << "demo1: " << demo1() << endl;
  cout << "demo2: " << demo2(2, 2, m, o_2, r_db_2, r_db_type_2) << endl;
  cout << "demo3: " << demo3(6, 10, M2, o_3, r_db_3, r_db_type_3) << endl;

  delete M2;
  delete m;
  return 0;
}
