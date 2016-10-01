/* short.c */

#ifdef _MSC_VER
//stupid warning ...  
//http://stackoverflow.com/questions/6880045/how-can-i-work-around-visual-c-2005s-decorated-name-length-exceeded-name-wa
#pragma warning(disable : 4503)  
#endif

#include <algorithm>
#include <iostream>
#include <vector>
#include <stdio.h>            /* C input/output                       */
#include <stdlib.h>           /* C standard library                   */
#include <glpk.h>             /* GNU GLPK linear/mixed integer solver */
#include <sstream>
#include <unordered_map>
#include "io/io.h"
#include "optmization/glpk_lp.h"
#include "optmization/model_glpk.h"
#include "reaction.h"
#include "blas/blas_utilities.h"
#include "model/stoichiometric_model.h"
#include "model/fba_adapter.h"
#include "biodb_service.h"



using namespace std;

string guessOriginal(string rxnEntry)
{
  vector<string> s = io::split(rxnEntry, '_');
  if (s.size() >= 3) {
    string fold1 = s[s.size() - 2];
    string fold2 = s[s.size() - 1];
    string rxnEntryOriginal = rxnEntry.substr(0, rxnEntry.size() - fold1.size() - fold2.size() - 2);
    //std::cout << "reaction " << rxnEntry << " maybe fold?" << rxnEntryOriginal << " folded -> " << fold1 << " ### " << fold2 << endl;
    return rxnEntryOriginal;
  }
  return rxnEntry;
}

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
    //cout << k << ": " << e << endl;
  }
  cout << "makeGlpLp - Metabolites: " << r << endl;

  cout << "makeGlpLp - Initialize Matrix" << endl;
  double **M = new double*[r];
  for (int i = 0; i < r; i++)
  {
    M[i] = new double[c];
    for (int j = 0; j < c; j++)
    {
      M[i][j] = 0;
    }
  }
  cout << "makeGlpLp - Initialize Reactions" << endl;
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

  cout << "makeGlpLp - Set LP Matrix" << endl;
  for (int i = 0; i < r; i++)
  {
    lp->set_row_bound(i, Op::EQUAL, 0);
    for (int j = 0; j < c; j++)
    {
      //cout << M[i][j] << " ";
      lp->set_matrix_value(i, j, M[i][j]);
      
    }
    //cout << endl;
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
  io::read_matrix_file(path,
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
  lp->set_obj_dir(Op::MAX);
  map<string, int> rxn_alias_index;
  for (int i = 0; i < rxn_alias.size(); i++)
  {
    rxn_alias_index.insert({ rxn_alias[i], i});
  }
  
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
  std::ostringstream strs;
  for (auto &e : rxn_alias)
  {
    strs << e.c_str() << ": " << flux_map[e] << endl;
  }
  io::WriteFile(strs.str(), "D:/tmp/lol.txt");
  cout << ini << " -> " << end << endl;
  cout << end - ini << endl;
  cout << cols / (end - ini) << " FBA/s" << endl;
  delete lp;

  return z;
}

double demo8(const string path, const string obj)
{
  model::StoichiometricModel sm = *io::ReadMatrixFile2(path);
  model::FluxBalanceAnalysisAdapter fba(sm);
  //int biomass_index = fba.get_lp()->get_col_index("R_Biomass_Ecoli_core_N__w_GAM_");
  int biomass_index = fba.get_lp()->get_col_index(obj);
  fba.get_lp()->set_obj_coef(biomass_index, 1.0);
  int n_rxn = fba.get_lp()->get_cols();
  vector<double> flux_values(n_rxn);

  double ini = blas::WallTime();
  for (int i = 0; i < n_rxn; i++)
  {
    fba.knock_out_reaction(i);
    double z = fba.solve();
    flux_values[i] = z;
    fba.reset_knockouts();
  }
  double end = blas::WallTime();
  
  for (int i = 0; i < n_rxn; i++)
  {
    cout << sm.get_rxn_alias(i) << ": " << flux_values[i] << endl;
  }
  
  cout << ini << " -> " << end << endl;
  cout << end - ini << endl;
  cout << n_rxn / (end - ini) << " FBA/s" << endl;

  return fba.solve();
}

double demo9(const string path, const string obj)
{
  model::StoichiometricModel sm = *io::ReadMatrixFile2(path);
  model::FluxBalanceAnalysisAdapter fba(sm);
  int biomass_index = fba.get_lp()->get_col_index(obj);
  fba.get_lp()->set_obj_coef(biomass_index, 1.0);
  int n_rxn = fba.get_lp()->get_cols();
  vector<double> flux_values(n_rxn);
  model::FluxBalanceSolution* fbs = fba.fba();
  //fba.solve2();
  return fbs->obj;
}

double demo10(const string path, const string obj)
{
  model::StoichiometricModel sm = *io::ReadMatrixFile2(path);
  model::FluxBalanceAnalysisAdapter fba(sm);
  int biomass_index = fba.get_lp()->get_col_index(obj);
  fba.get_lp()->set_obj_coef(biomass_index, 1.0);
  int n_rxn = fba.get_lp()->get_cols();
  vector<double> flux_values(n_rxn);
  model::FluxBalanceSolution* fbs_fbs = fba.fba();
  model::FluxBalanceSolution* fbs_pfba = fba.pfba(biomass_index);
  cout << " FBA flux sum " << fbs_fbs->flux_sum() << endl;
  cout << "pFBA flux sum " << fbs_pfba->flux_sum() << endl;
  return fbs_fbs->obj;
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

int testRead(int argc, char** argv)
{
  set<long> substrates;
  io::ReadProblemFile("D:/tmp/example_config.tsv", &substrates);
  return 0;
}

const double EPSILON = 1e-9;

bool __inline isEqual(double a, double b)
{
  if (abs(a - b) < EPSILON) {
    return false;
  }

  return true;
}

class Awesomeness
{
public:
  set<long> cpdIds;
  map<int, string> cpd_names;
  map<string, string> cpdOriginal_to_alias;
  map<string, string> rxnOriginal_to_alias;
  map<string, string> rxnOriginal_to_rev;
  map<string, string> rxn_rev_alias;
  map<string, string> rxn_alias_orig;
  map<int, long> drainIndexToCpd;
  vector<bio::Reaction> rxnList;
  set<string> rxn_;
  vector<string> rxn_names;
  set<long> nonnative;
  optimization::GlpkLp* lp;
  
  void registerReaction(bio::Reaction rxn)
  {
    string rxnEntry = rxn.entry;
    string rxnO = rxn.entry;
    std::replace(rxnO.begin(), rxnO.end(), ':', '_');
    std::replace(rxnO.begin(), rxnO.end(), '-', '_');
    rxnOriginal_to_alias[rxnEntry] = rxnO;
    rxnList.push_back(rxn);
    rxn_names.push_back(rxnO);
    rxn_.insert(rxnO);
    bio::Reaction rxn_rev(-1, rxnO + "_rev");
    for (auto e : rxn.get_lhs_stoichiometry())
    {
      cpdIds.insert(e.first);
      rxn_rev.add_rhs_stoichiometry(e.first, e.second);
    }
    for (auto e : rxn.get_rhs_stoichiometry())
    {
      cpdIds.insert(e.first);
      rxn_rev.add_lhs_stoichiometry(e.first, e.second);
    }

    rxnList.push_back(rxn_rev);
    rxn_names.push_back(rxn_rev.entry);
    rxnOriginal_to_rev[rxnO] = rxn_rev.entry;
    std::cout << "added reaction " << rxn.entry << " -> " << rxnO << " >> " << rxnOriginal_to_rev[rxnO] << " total: " << rxnList.size() << std::endl;
  }

  void registerMetabolite(bio::Metabolite cpd)
  {
    //std::cout << cpd << std::endl;
    if (cpdIds.find(cpd.id) != cpdIds.end())
    {
      string cpdEntry = cpd.entry;
      string cpdO = cpd.entry;
      std::replace(cpdO.begin(), cpdO.end(), ':', '_');
      std::replace(cpdO.begin(), cpdO.end(), '-', '_');
      cpdOriginal_to_alias.insert({ cpdEntry, cpdO });
      
      bio::Reaction d;
      d.add_rhs_stoichiometry(cpd.id, 1.0);
      drainIndexToCpd[(int) rxnList.size()] = cpd.id;
      rxnList.push_back(d);
      std::ostringstream ss;
      ss << cpdO;
      std::string entry = ss.str();
      rxn_names.push_back(entry);

      cpd_names.insert({ cpd.id, cpdO });

      std::cout << "added metabolite " << cpd.entry << " -> " << cpdO << " total: " << cpd_names.size() << "/" <<cpdIds.size() << std::endl;
    }
    else
    {
      std::cerr << "metabolite not fond " << cpd.id << std::endl;
    }

  }

  void build(set<long> substrates)
  {
    if (rxnList.size() == 0) {
      std::cout << "empty network" << std::endl;
      return;
    }

    for (auto e : rxnOriginal_to_alias)
    {
      string entry = e.first;
      string alias = e.second;
      string rev = rxnOriginal_to_rev[alias];
      rxn_rev_alias[rev] = alias;
      rxn_alias_orig[alias] = entry;
    }

    lp = makeGlpLp(rxnList, cpd_names, rxn_names);
    lp->set_obj_dir(Op::MIN);
    for (auto rxnId : rxn_) {
      {
        lp->set_col_bound(0, Op::GE, lp->get_col_index(rxnId), Op::GE, 1000);
        lp->set_col_bound(0, Op::GE, lp->get_col_index(rxnOriginal_to_rev[rxnId]), Op::GE, 1000);
      }
    }

    std::cout << "open substrates" << std::endl;
    for (auto cpdId : substrates) {
      if (cpd_names.find(cpdId) != cpd_names.end())
      {
        std::cout << cpd_names[cpdId] << std::endl;
        lp->set_col_bound(-1000, Op::GE, lp->get_col_index(cpd_names[cpdId]), Op::GE, 1000);
      }
    }

    std::cout << "find non native substrates" << std::endl;
    for (auto cpdId : cpdIds)
    {
      if (substrates.find(cpdId) == substrates.end())
      {
        nonnative.insert(cpdId);
      }
    }

    std::cout << "flush non native substrates" << std::endl;
    for (auto cpdId : nonnative) {
      std::cout << cpd_names[cpdId] << std::endl;
      lp->set_col_bound(0, Op::GE, lp->get_col_index(cpd_names[cpdId]), Op::GE, 1000);
    }

    //lp->write_lp("D:/tmp/lp_par_temp_zero.txt");
  }

  map<map<long, double>, set<long>> batch(string target, set<long> sols, map<long, set<string>> solutionMap)
  {
    map<map<long, double>, set<long>> netMap;
    set<set<string>> sol;
    /*string target = t.first;
    if (sol.find(t.first) == sol.end())
    {
      sol.insert({ t.first, set<set<string>>() });
    }*/
    map<double, set<string>> revmap;
    revmap.insert({ -1, set<string>() });
    revmap.insert({  1, set<string>() });
    lp->set_col_bound(1, Op::GE, lp->get_col_index(cpdOriginal_to_alias[target]), Op::GE, 1);
    std::cout << target << std::endl;
    for (auto e : sols)
    {
      set<string> s = solutionMap[e];
      if (sol.find(s) == sol.end()) {
        //shudown all
        for (auto rxnId : rxn_) {
          {
            lp->set_col_bound(0, Op::GE, lp->get_col_index(rxnId), Op::GE, 0);
            lp->set_col_bound(0, Op::GE, lp->get_col_index(rxnOriginal_to_rev[rxnId]), Op::GE, 0);
          }
        }

        
        for (auto rxnEntry : s) {
          
          string rxnAlias = rxnOriginal_to_alias[rxnEntry];

          if (rxnAlias.size() == 0)
          {
            rxnAlias = rxnOriginal_to_alias[guessOriginal(rxnEntry)];
          }
          //std::cout << " S:" << rxnEntry << " A:" << rxnAlias << std::endl;
          string rxnAliasR = rxnOriginal_to_rev[rxnAlias];

          

          lp->set_obj_coef(lp->get_col_index(rxnAlias), 1.0);
          lp->set_obj_coef(lp->get_col_index(rxnAliasR), 1.0);
          lp->set_col_bound(0, Op::GE, lp->get_col_index(rxnAlias), Op::GE, 1000);
          lp->set_col_bound(0, Op::GE, lp->get_col_index(rxnAliasR), Op::GE, 1000);
          
        }
        //std::cout << std::endl;
        sol.insert(s);
        //lp->write_lp("D:/tmp/lp_par_temp.txt");
        //std::cout << "solve ..." << std::endl;
        double obj = lp->solve_exact();
        //double obj = lp->solve();
        int status = glp_get_status(lp->get_lp());
        if (status == GLP_FEAS || status == GLP_OPT) {
          map<long, double> net;

          //std::cout << "Objective: " << obj << std::endl;
          for (int i = 0; i < lp->get_cols(); i++)
          {
            double v = lp->get_col_prim(i);
            //if (isEqual(v, 0.0))
            if (v != 0.0)
            {
              string rxn_maybe_rev = rxn_names[i];
              //std::cout << rxn_maybe_rev << ": " << v << std::endl;
              string alias = rxn_maybe_rev;
              string rxnEntry = "";
              bool lr = true;
              if (rxn_rev_alias.find(rxn_maybe_rev) != rxn_rev_alias.end())
              {
                alias = rxn_rev_alias[rxn_maybe_rev];
                lr = false;
              }
              if (rxn_alias_orig.find(alias) != rxn_alias_orig.end())
              {
                rxnEntry = rxn_alias_orig[alias];
              }
              else
              {
                net[drainIndexToCpd[i]] = v;
              }


              if (rxnEntry.size() > 0)
              {
                std::cout << "[" << rxnEntry << "]" << rxn_names[i] << std::endl;
                if (lr)
                {
                  revmap[-1].insert(rxnEntry);
                }
                else
                {
                  revmap[1].insert(rxnEntry);
                }
                //std::cout << rxnEntry << " >> " << rxn_names[i] << ": " << v << std::endl;
              }
              //else is drain !?

            }
          }

          if (netMap.find(net) == netMap.end())
          {
            netMap.insert({ net, set<long>() });
          }
          netMap[net].insert(e);
        }
        else
        {
          std::cout << "WARN: not feasible lp status: " << status << std::endl;
        }


        
        for (auto rxnEntry : s) {
          string rxnAlias = rxnOriginal_to_alias[rxnEntry];
          string rxnAliasR = rxnOriginal_to_rev[rxnAlias];
          lp->set_obj_coef(lp->get_col_index(rxnAlias), 0);
          lp->set_obj_coef(lp->get_col_index(rxnAliasR), 0);
          //lp->set_col_bound(0, Op::GE, lp->get_col_index(rxnAlias), Op::GE, 1000);
          //lp->set_col_bound(0, Op::GE, lp->get_col_index(rxnAliasR), Op::GE, 1000);
          //std::cout << " " << rxnEntry;
        }

      }
      
    }
    
    lp->set_obj_coef(lp->get_col_index(cpdOriginal_to_alias[target]), 0);
    lp->set_col_bound(0, Op::GE, lp->get_col_index(cpdOriginal_to_alias[target]), Op::GE, 1000);

    for (auto e : revmap[-1])
    {
      std::cout << e << "\tRL" <<std::endl;
    }
    for (auto e : revmap[1])
    {
      std::cout << e << "\tLR" << std::endl;
    }

    return netMap;
  }
};

bio::Reaction getReaction(biodb::BiodbService service, string db, string rxnEntry)
{
  bio::Reaction rxn = service.getReaction(db, rxnEntry);
  if (rxn.entry.size() == 0)
  {
    //try check if fold
    vector<string> s = io::split(rxnEntry, '_');
    if (s.size() >= 3) {
      string fold1 = s[s.size() - 2];
      string fold2 = s[s.size() - 1];
      string rxnEntryOriginal = rxnEntry.substr(0, rxnEntry.size() - fold1.size() - fold2.size() - 2);
      //std::cout << "reaction " << rxnEntry << " maybe fold?" << rxnEntryOriginal << " folded -> " << fold1 << " ### " << fold2 << endl;
      bio::Reaction frxn = service.getReaction(db, rxnEntryOriginal);
      if (frxn.entry.size() == 0)
      {
        std::cout << "reaction " << rxnEntry << " not found with fold atempt: " << rxnEntryOriginal << endl;
      }
      else
      {
        rxn = frxn;
      }
    }
    else
    {
      std::cout << "reaction " << rxnEntry << " not found" << endl;
    }

  }
  return rxn;
}

void testLoadReactions()
{
  biodb::BiodbService service;
  set<string> hnet;
  hnet.insert("R00200");
  hnet.insert("META:RXN-12411");
  hnet.insert("META:RXN-12411_1563457_1610842");
  std::cout << "loading reactions" << std::endl;
  string db = "MetaCyc";
  for (auto rxnEntry : hnet) {
    bio::Reaction rxn = getReaction(service, db, rxnEntry);
    if (rxn.entry.size() != 0)
    {
      std::cout << rxn << std::endl;
    }
  }
//META:RXN-12411_1563457_1610842
}

void testSomeMaps(int argc, char** argv)
{
  map<map<long, double>, set<long>> netMap;
  map<long, double> net1;
  net1.insert({ 1, -1 });
  net1.insert({ 2, -1 });
  net1.insert({ 3,  1 });
  net1.insert({ 4,  1 });
  map<long, double> net2;
  net2.insert({ 4,  1 });
  net2.insert({ 1, -1 });
  net2.insert({ 3,  1 });
  net2.insert({ 2, -1 });
  map<long, double> net3;
  net3.insert({ 1,  1 });
  net3.insert({ 2,  1 });
  net3.insert({ 3, -1 });
  net3.insert({ 4, -1 });

  if (netMap.find(net1) != netMap.end())
  {
    netMap.insert({ net1, set<long>() });
  }
  netMap[net1].insert(1000);

  if (netMap.find(net2) != netMap.end())
  {
    netMap.insert({ net2, set<long>() });
  }
  netMap[net2].insert(1050);

  if (netMap.find(net3) != netMap.end())
  {
    netMap.insert({ net3, set<long>() });
  }
  netMap[net3].insert(1100);

  for (auto e : netMap)
  {
    bio::Reaction rxn;
    
    map<long, double> net = e.first;
    rxn.set_stoichiometry(net);
    set<long> sols = e.second;
    cout << rxn << " " << io::toString(sols) << std::endl;
    
  }
}

int main(int argc, char** argv)
{
  //D:/tmp/synth
  //string solFile = "D:/tmp/demo_solutions.tsv";
  //string cfgFile = "D:/tmp/demo_config.tsv";
  //string outPath = "D:/tmp/demo_config.tsv";
  string solFile = "D:/tmp/synth/lycopene_MetaCyc_r_9_50_sol.tsv";
  string cfgFile = "D:/tmp/synth/lycopene_MetaCyc_r_9_50_cfg.tsv";
  //string solFile = "D:/tmp/synth/vanillin_MetaCyc_r_9_50_sol.tsv";
  //string cfgFile = "D:/tmp/synth/vanillin_MetaCyc_r_9_50_cfg.tsv";
  string outPath = "D:/tmp/test_net2.tsv";
  //string solFile = "D:/tmp/synth/b.tsv";
  //string cfgFile = "D:/tmp/synth/a.tsv";
  glp_term_out(GLP_OFF);
  Awesomeness aa;
  //biodb::wut("D:/tmp/biodb_pyk.json");
  map<long, set<string>> solutions;
  //set<string> rxn_;
  set<string> hnet;
  map<string, set<long>> targetToSolutions;
  io::ReadSolutionFile(solFile, &hnet, &solutions, &targetToSolutions);
  set<long> substrates;
  //set<long> nonnative;
  io::ReadProblemFile(cfgFile, &substrates);
  
  /*
  hnet.insert("R00200");
  hnet.insert("R00658");
  hnet.insert("R00431");
  hnet.insert("R01518");
  */
  for (auto e : hnet) {
    std::cout << e << std::endl;
  }
  
  biodb::BiodbService service;

  std::cout << "loading reactions" << std::endl;
  for (auto rxnEntry : hnet) {
    //bio::Reaction rxn1 = service.getReaction("LigandReaction", "R00200");
    bio::Reaction rxn = getReaction(service, "MetaCyc", rxnEntry);
    if (rxn.entry.size() != 0) {
      aa.registerReaction(rxn);
    }
  }
  //service.getReaction(10);
  std::cout << "loading metabolites" << std::endl;
  for (auto cpdId : aa.cpdIds) {

    bio::Metabolite cpd = service.getMetabolite("*", cpdId);
    aa.registerMetabolite(cpd);
    /*
    if (substrates.find(cpdId) == substrates.end()) {
    nonnative.insert(cpdId);
    }
    string cpdEntry = cpd.entry;
    std::replace(cpd.entry.begin(), cpd.entry.end(), ':', '_');
    std::replace(cpd.entry.begin(), cpd.entry.end(), '-', '_');
    cpdOriginal_to_alias.insert({ cpdEntry, cpd.entry });
    std::cout << cpd << std::endl;
    bio::Reaction d;
    d.add_rhs_stoichiometry(cpdId, 1.0);
    rxnList.push_back(d);
    std::ostringstream ss;
    ss << cpd.entry;
    std::string entry = ss.str();
    rxn_names.push_back(entry);

    cpd_names.insert({ cpdId, cpd.entry });
    */
  }
  
  /*
  bio::Reaction rxn1;
  rxn1.add_lhs_stoichiometry(58310, 1.0); //C00002
  rxn1.add_lhs_stoichiometry(370761, 1.0); //C00022
  rxn1.add_rhs_stoichiometry(1086446, 1.0); //C00008
  rxn1.add_rhs_stoichiometry(259799, 1.0); //C00074
  rxnList.push_back(d1);
  rxnList.push_back(d2);
  rxnList.push_back(d3);
  rxnList.push_back(d4);

  cpd_names.insert({ 370761, "C00022" });
  cpd_names.insert({ 1086446, "C00008" });
  cpd_names.insert({ 259799, "C00074" });
  */

  
  aa.build(substrates);
  /*
  if (rxnList.size() == 0) {
    std::cout << "empty network" << std::endl;
    return 1;
  }
  */
  /*
  optimization::GlpkLp* lp = makeGlpLp(rxnList, cpd_names, rxn_names);
  lp->set_obj_dir(Op::MIN);
  //lp->set_col_bound(1, Op::EQUAL, lp->get_col_index("R00200"), Op::EQUAL, 1);
  
  for (auto rxnId : rxn_) {
    {
      lp->set_col_bound(-1000, Op::GE, lp->get_col_index(rxnId), Op::GE, 1000);
    }
  }
  std::cout << "open substrates" << std::endl;
  for (auto cpdId : substrates) {
    if (cpd_names.find(cpdId) != cpd_names.end())
    {
      std::cout << cpd_names[cpdId] << std::endl;
      lp->set_col_bound(-1000, Op::GE, lp->get_col_index(cpd_names[cpdId]), Op::GE, 1000);
    }
  }
  std::cout << "flush non native substrates" << std::endl;
  for (auto cpdId : nonnative) {
    std::cout << cpd_names[cpdId] << std::endl;
    lp->set_col_bound(0, Op::GE, lp->get_col_index(cpd_names[cpdId]), Op::GE, 1000);
  }
  //set internal as flush
  map<string, set<set<string>>> sol;
  */
  for (auto t : targetToSolutions)
  {
    string target = t.first;
    std::replace(target.begin(), target.end(), ':', '_');
    std::replace(target.begin(), target.end(), '-', '_');
    map<map<long, double>, set<long>> netMap = aa.batch(t.first, t.second, solutions);
    //string path = "/nnet_" + target + ".tsv";
    io::WriteNetMap(netMap, outPath);
    /*
    for (auto e : netMap)
    {
      bio::Reaction rxn;
      map<long, double> net = e.first;
      rxn.set_stoichiometry(net);
      set<long> sols = e.second;
      cout << rxn << " " << io::toString(sols) << std::endl;
    }
    
    string target = t.first;
    if (sol.find(t.first) == sol.end())
    {
      sol.insert({t.first, set<set<string>>()});
    }
    lp->set_obj_coef(lp->get_col_index(cpdOriginal_to_alias[target]), 1.0);
    lp->set_col_bound(1, Op::GE, lp->get_col_index(cpdOriginal_to_alias[target]), Op::GE, 1);
    std::cout << t.first << std::endl;
    for (auto e : t.second)
    {
      set<string> s = solutions[e];
      if (sol[target].find(s) == sol[target].end()) {
        //shudown all
        for (auto rxnId : rxn_) {
          {
            lp->set_col_bound(0, Op::GE, lp->get_col_index(rxnId), Op::GE, 0);
          }
        }
        for (auto rxnEntry : s) {
          lp->set_col_bound(-1000, Op::GE, lp->get_col_index(rxnOriginal_to_alias[rxnEntry]), Op::GE, 1000);
          std::cout << " " << rxnEntry;
        }
        std::cout << std::endl;
        sol[target].insert(s);
        double obj = lp->solve();
        std::cout << "Objective: " << obj << std::endl;
        for (int i = 0; i < lp->get_cols(); i++)
        {
          double v = lp->get_col_prim(i);
          if (v != 0.0)
          {
            std::cout << rxn_names[i] << ": " << v << std::endl;
          }
        }
        
      }
      
    }
    lp->write_lp("D:/lp_temp.txt");
    lp->set_obj_coef(lp->get_col_index(cpdOriginal_to_alias[target]), 0);
    lp->set_col_bound(0, Op::GE, lp->get_col_index(cpdOriginal_to_alias[target]), Op::GE, 1000);
    */
  }
  
  /*
  lp->set_col_bound(-10, Op::GE, lp->get_col_index("C00002"), Op::GE, 10);
  lp->set_col_bound(-10, Op::GE, lp->get_col_index("C00022"), Op::GE, 10);
  lp->set_col_bound(-10, Op::GE, lp->get_col_index("C00074"), Op::GE, 10);
  lp->set_col_bound(-10, Op::GE, lp->get_col_index("C00008"), Op::GE, 10);
  */
  
  
  

  //delete lp;
  
  return 0;
}

/*
 * cols -> reactions
 * rows -> metabolites
 */
int main_demo(int argc, char** argv)
{
  glp_term_out(GLP_OFF);
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
  //const string file = "D:/var/biomodels/matrix/Ec_core_flux1_no_b.txt";
  //const string obj = "R_Biomass_Ecoli_core_N__w_GAM_";
  const string file = "D:/var/biomodels/matrix/iMM904_no_b.txt";
  const string obj = "R_biomass_SC5_notrace";
  //const string file(argv[1]);
  //const string obj(argv[2]);
  //cout << "demo7: " << demo7(file, obj) << endl;
  //cout << "demo8: " << demo8(file, obj) << endl;
  //cout << "demo9: " << demo9(file, obj) << endl;
  cout << "demo10: " << demo10(file, obj) << endl;
  delete M2;
  delete m;
  return 0;
}
