/* short.c */
#include <iostream>
#include <vector>
#include <stdio.h>            /* C input/output                       */
#include <stdlib.h>           /* C standard library                   */
#include <glpk.h>             /* GNU GLPK linear/mixed integer solver */
#include "optmization/glpk_lp.h"
#include "optmization/model_glpk.h"
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
  lp->set_row_alias(0, "p");
  lp->set_row_bound(0, Op::LE, 1.0); // <= 1.0
  lp->set_row_alias(1, "q");
  lp->set_row_bound(1, Op::LE, 2.0); // <= 2.0

  lp->set_col_alias(0, "x1");
  lp->set_obj_coef(0, 0.6);
  lp->set_col_alias(1, "x2");
  lp->set_obj_coef(1, 0.5);

  const double matrix[2][2] = {
      { 1.0, 2.0 },
      { 3.0, 1.0 },
  };

  lp->set_matrix_value(0, 0, 1.0); lp->set_matrix_value(0, 1, 2.0);
  lp->set_matrix_value(1, 0, 3.0); lp->set_matrix_value(1, 1, 1.0);
  lp->load_matrix();
  double opt = lp->solve();
  lp->write_lp("D:/tmp_lp_2.txt");
  delete lp;
  return opt;
}

double demo4()
{
  const int r = 6;
  const int c = 10;
  optimization::GlpkLp* lp = new optimization::GlpkLp(r, c);
  lp->set_name("network");
  lp->set_obj_dir(Op::MAX);

  const string row_names[r] = { "A", "B", "C", "D", "E", "P" };
  const string col_names[c] = { "R1", "R2", "R3", "R4", "R5",
                                "R6", "R7", "R8", "R9", "R10" };
  double const M[r][c] = {
      { 1, 0,  0,  0, -1, -1, -1,  0,  0,  0 }, //A
      { 0, 1,  0,  0,  1,  0,  0, -1, -1,  0 }, //B
      { 0, 0,  0,  0,  0,  1,  0,  1,  0, -1 }, //C
      { 0, 0,  0,  0,  0,  0,  1,  0,  0, -1 }, //D
      { 0, 0,  0, -1,  0,  0,  0,  0,  0,  1 }, //E
      { 0, 0, -1,  0,  0,  0,  0,  0,  1,  1 }, //P
  };

  
  lp->set_col_alias(0, col_names[0]);
  lp->set_col_alias(1, col_names[1]);
  lp->set_col_alias(2, col_names[2]);
  lp->set_col_alias(3, col_names[3]);
  lp->set_col_bound(-10, Op::LE, 0, Op::LE, 10);
  lp->set_col_bound(-10, Op::LE, 1, Op::LE, 10);
  lp->set_col_bound(0, Op::UNBOUNDED, 2, Op::UNBOUNDED, 0);
  lp->set_col_bound(0, Op::LE, 3, Op::UNBOUNDED, 0);

  for (int j = 4; j < c; j++)
  {
    lp->set_col_alias(j, col_names[j]);
    lp->set_col_bound(-1000, Op::UNBOUNDED, j, Op::UNBOUNDED, 1000);
  }


  
  for (int i = 0; i < r; i++)
  {
    lp->set_obj_coef(i, 0.0);
    lp->set_row_alias(i, row_names[i]);
    lp->set_row_bound(i, Op::EQUAL, 0.0);
    for (int j = 0; j < c; j++)
    {
      lp->set_matrix_value(i, j, M[i][j]);
    }
  }

  lp->set_obj_coef(2, 1.0);
  lp->load_matrix();
  lp->write_lp("D:/tmp_lp_3.txt");
  double opt = lp->solve();

  for (int j = 0; j < c; j++)
  {
    cout << col_names[j].c_str() << ": " << lp->get_col_prim(j) << endl;
  }

  delete lp;
  return opt;
}

optimization::GlpkLp* makeGlpLp(vector<bio::Reaction> rxnList,
                                map<int, string> cpdAlias,
                                vector<string> rxnAlias)
{
  int c = (int) rxnList.size();
  set<int> cpdSet;
  map<int, int> cpdArray;
  for (auto &rxn : rxnList)
  {
    for (auto &p : rxn.get_lhs_stoichiometry())
    {
      cpdSet.insert(p.first);
    }
    for (auto &p : rxn.get_rhs_stoichiometry())
    {
      cpdSet.insert(p.first);
    }
  }
  int r = (int)cpdSet.size();
  int k = 0;
  for (auto &e : cpdSet)
  {
    cpdArray.insert({ e, k++ });
    cout << k << ": " << e << endl;
  }
  cout << "Metabolites: " << r << endl;

  cout << "Initialize Matrix" << endl;
  double **M = new double*[r];
  for (int i = 0; i < r; i++)
  {
    M[i] = new double[c];
    for (int j = 0; j < c; j++)
    {
      M[i][j] = 0;
    }
  }
  cout << "Initialize Reactions" << endl;
  for (int i = 0; i < c; i++)
  {
    bio::Reaction rxn = rxnList[i];
    for (auto &p : rxn.get_lhs_stoichiometry())
    {
      int cpd = p.first;
      double value = -1 * p.second;
      int x = cpdArray[cpd];
      M[x][i] = value;
    }
    for (auto &p : rxn.get_rhs_stoichiometry())
    {
      int cpd = p.first;
      double value = p.second;
      int x = cpdArray[cpd];
      M[x][i] = value;
    }
  }

  optimization::GlpkModel* lp = new optimization::GlpkModel(r, c);
  lp->add_drain();

  cout << "Set LP Matrix" << endl;
  for (int i = 0; i < r; i++)
  {
    lp->set_row_bound(i, Op::EQUAL, 0);
    for (int j = 0; j < c; j++)
    {
      cout << M[i][j] << " ";
      lp->set_matrix_value(i, j, M[i][j]);
      
    }
    cout << endl;
  }
  lp->load_matrix();

  for (int i = 0; i < c; i++)
  {
    lp->set_col_alias(i, rxnAlias[i]);
    //reversible are unbound
    //[0, +inf[
    //[-inf, 0]
    //lp->set_col_bound(-1000, Op::UNBOUNDED, j, Op::UNBOUNDED, 1000);
  }
  
  return lp;
}

double demo5()
{
  const int r = 6;
  const int c = 10;
  double const M[r][c] = {
      { 1, 0, 0, 0, -1, -1, -1, 0, 0, 0 }, //A
      { 0, 1, 0, 0, 1, 0, 0, -1, -1, 0 }, //B
      { 0, 0, 0, 0, 0, 1, 0, 1, 0, -1 }, //C
      { 0, 0, 0, 0, 0, 0, 1, 0, 0, -1 }, //D
      { 0, 0, 0, -1, 0, 0, 0, 0, 0, 1 }, //E
      { 0, 0, -1, 0, 0, 0, 0, 0, 1, 1 }, //P
  };
  int const R[10] = { 0, 1, 0, 0, 0, 0, 0, 1, 0, 0 };
  const string row_names[r] = { "A", "B", "C", "D", "E", "P" };
  const string col_names[c] = { "R1", "R2", "R3", "R4", "R5",
    "R6", "R7", "R8", "R9", "R10" };
  vector<bio::Reaction> rxnList;
  map<int, string> cpd_names;
  vector<string> rxn_names;
  for (int i = 0; i < c; i++)
  {
    rxn_names.push_back(col_names[i]);
    bio::Reaction rxn;
    for (int j = 0; j < r; j++)
    {
      double v = M[j][i];
      if (v > 0)
      {
        rxn.add_rhs_stoichiometry(j, v);
      }
      else if (v < 0)
      {
        rxn.add_lhs_stoichiometry(j, -1 * v);
      }
    }
    if (R[i])
    {
      rxn.set_reversible(true);
    }
    rxnList.push_back(rxn);
  }
  for (int i = 0; i < r; i++)
  {
    cpd_names.insert({ i, row_names[i] });
  }
  
  std::cout << "Rxn: " << rxnList.size() << endl;
  for (auto &e : rxnList)
  {
    cout << e << endl;
  }
  
  optimization::GlpkLp* lp = makeGlpLp(rxnList, cpd_names, rxn_names);
  lp->write_lp("D:/tmp_lp_5.txt");
  double z = lp->solve();
  delete lp;
  return z;
}

double demo6()
{
  vector<bio::Reaction> rxnList;
  map<int, string> cpd_names;
  vector<string> rxn_names;
  bio::Reaction rxn1;
  rxn1.add_lhs_stoichiometry(  58310, 1.0); //C00002
  rxn1.add_lhs_stoichiometry( 370761, 1.0); //C00022
  rxn1.add_rhs_stoichiometry(1086446, 1.0); //C00008
  rxn1.add_rhs_stoichiometry( 259799, 1.0); //C00074

  rxnList.push_back(rxn1);
  rxn_names.push_back("R00200");
  cpd_names.insert({ 58310, "C00002" });
  cpd_names.insert({ 370761, "C00022" });
  cpd_names.insert({ 1086446, "C00008" });
  cpd_names.insert({ 259799, "C00074" });
  optimization::GlpkLp* lp = makeGlpLp(rxnList, cpd_names, rxn_names);
  delete lp;
  return 0;
}

double demo7(const string path, const string obj)
{
  double** matrix;
  int* rev;
  double* lb;
  double* ub;
  int rows;
  int cols;
  vector<string> rxn_alias;
  vector<string> cpd_alias; //Ec_core_flux1_no_b
  blas::read_matrix_file(path,
                         &rows, &cols, &matrix, &rev, &ub, &lb, 
                         &rxn_alias, &cpd_alias);
  cout << "Metabolites: " << cpd_alias.size() << endl;
  cout << "Reactions  : " << rxn_alias.size() << endl;
  
  int i = 0;
  for (auto &e : rxn_alias)
  {
    //cout << ++i << "[" << e.c_str() << "]" << endl;
  }
  i = 0;
  for (auto &e : cpd_alias)
  {
    //cout << ++i << "[" <<e.c_str() << "]" << endl;
  }

  optimization::GlpkLp* lp = new optimization::GlpkLp(rows, cols);
  lp->set_name("model");
  for (int i = 0; i < rows; i++)
  {
    lp->set_row_alias(i, cpd_alias[i]);
    lp->set_row_bound(i, Op::EQUAL, 0);
    for (int j = 0; j < cols; j++)
    {
      //cout << matrix[i][j] << " ";
      lp->set_matrix_value(i, j, matrix[i][j]);
    }
    //cout << endl;
  }
  for (int i = 0; i < cols; i++)
  {
    lp->set_col_alias(i, rxn_alias[i]);
    lp->set_col_bound(lb[i], Op::LE, i, Op::LE, ub[i]);
  }
  lp->load_matrix();

  map<string, int> rxn_alias_index;
  for (int i = 0; i < rxn_alias.size(); i++)
  {
    rxn_alias_index.insert({ rxn_alias[i], i});
  }
  lp->set_obj_dir(Op::MAX);
  //R_biomass_SC5_notrace
  //R_Biomass_Ecoli_core_N__w_GAM_
  lp->set_obj_coef(rxn_alias_index[obj], 1.0);
  //lp->write_lp("D:/tmp_lp_7.txt");
  double z = lp->solve();
  glp_prob* glp_lp = lp->get_lp();
  map<string, double> flux_map;
  double ini = blas::WallTime();
  for (int i = 1; i <= cols; i++)
  {
    int type_ = glp_get_col_type(glp_lp, i);
    double lb_ = glp_get_col_lb(glp_lp, i);
    double ub_ = glp_get_col_ub(glp_lp, i);
    glp_set_col_bnds(glp_lp, i, GLP_FX, 0.0, 0.0);
    z = lp->solve();
    flux_map.insert({ rxn_alias[i - 1], z });
    glp_set_col_bnds(glp_lp, i, type_, lb_, ub_);
  }
  double end = blas::WallTime();
  for (auto &e : rxn_alias)
  {
    cout << e.c_str() << ": " << flux_map[e] << endl;
  }

  cout << ini << " -> " << end << endl;
  cout << end - ini << endl;
  cout << cols / (end - ini) << " FBA/s" << endl;
  delete lp;

  return z;
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
  glp_write_lp(lp, NULL, "D:/tmp_lp_1.txt");
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
  cout << "demo0: " << demo0() << endl; //glp base
  cout << "demo0: " << demo0() << endl; //glp base
  cout << "demo1: " << demo1() << endl;
  cout << "demo2: " << demo2(2, 2, m, o_2, r_db_2, r_db_type_2) << endl;
  cout << "demo3: " << demo3(6, 10, M2, o_3, r_db_3, r_db_type_3) << endl;
  cout << "demo4: " << demo4() << endl; //glp class
  cout << "demo5: " << demo5() << endl; //make glp
  cout << "demo6: " << demo6() << endl; //dummy kegg reaction
  //const string file = "D:/home/data/sbml/biomodels/matrix/iMM904_no_b.txt";
  //const string obj = "R_biomass_SC5_notrace";
  //const string file = "D:/home/data/sbml/biomodels/matrix/Ec_core_flux1_no_b.txt";
  //const string obj = "R_Biomass_Ecoli_core_N__w_GAM_";
  const string file(argv[1]);
  const string obj(argv[2]);
  cout << "demo7: " << demo7(file, obj) << endl;

  delete M2;
  delete m;
  return 0;
}

