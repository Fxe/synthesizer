#include "fba_adapter.h"
#include <iostream>
#include <cmath>

using namespace model;
using namespace std;

FluxBalanceAnalysisAdapter::FluxBalanceAnalysisAdapter(
  const StoichiometricModel& sm)
{
  int rows = sm.get_matrix()->get_rows();
  int cols = sm.get_matrix()->get_cols();
  lp_ = new optimization::GlpkLp(rows, cols);
  
  lp_->set_name("model");
  for (int i = 0; i < rows; i++)
  {
    lp_->set_row_alias(i, sm.get_cpd_alias(i));
    lp_->set_row_bound(i, Op::EQUAL, 0);
    for (int j = 0; j < cols; j++)
    {
      const double v = sm.get_matrix()->get(i, j);
      lp_->set_matrix_value(i, j, v);
    }
  }
  default_bounds_ = new vector<pair<double, double>>(cols);
  dirty_bounds_ = new vector<int>(cols);
  for (int i = 0; i < cols; i++)
  {
    lp_->set_col_alias(i, sm.get_rxn_alias(i));
    pair<double, double> b = sm.get_rxn_bounds(i);
    lp_->set_col_bound(b.first, Op::LE, i, Op::LE, b.second);
    (*default_bounds_)[i] = { b.first, b.second };
    (*dirty_bounds_)[i] = 0;
  }
  //lp_->set_obj_coef(1, 1.0);
  lp_->set_obj_dir(Op::MAX);
  lp_->load_matrix();
}

void FluxBalanceAnalysisAdapter::knock_out_reaction(int rxnIndex)
{
  if (rxnIndex < lp_->get_cols())
  {
    lp_->set_col_bound(0.0, Op::LE, rxnIndex, Op::LE, 0.0);
    (*dirty_bounds_)[rxnIndex] = 1;
    cout << rxnIndex << " going down !" << endl;
  }
}

void FluxBalanceAnalysisAdapter::knock_out_reaction(string rxnId)
{
  int i = lp_->get_col_index(rxnId);
  knock_out_reaction(i);
}

void FluxBalanceAnalysisAdapter::reset_knockouts()
{
  for (int i = 0; i < dirty_bounds_->size(); i++)
  {
    if ((*dirty_bounds_)[i])
    {
      const double lb = (*default_bounds_)[i].first;
      const double ub = (*default_bounds_)[i].second;
      lp_->set_col_bound(lb, Op::LE, i, Op::LE, ub);
    }
  }
}



double FluxBalanceAnalysisAdapter::solve()
{
  return lp_->solve();
}

void FluxBalanceAnalysisAdapter::solve2()
{
  
  //lp_->set_obj_dir(math::Op::MIN);
  
  //lp_->set_obj_coef(1, 1.0);
  //do initial solve
  double pivot = lp_->solve();
  lp_->after_solve();
  set<int> test;
  for (int i = 0; i < lp_->get_cols(); i++)
  {
    double pv = lp_->get_col_prim(i);
    if (pv != 0.0)
    {
      test.insert(i);
    }
  }
  lp_->set_col_bound(pivot, Op::EQUAL, 1, Op::EQUAL, pivot);
  //now test alternative routes
  //restricted upper bound

}

optimization::GlpkLp* FluxBalanceAnalysisAdapter::get_lp() const
{
  return lp_;
}

FluxBalanceSolution* FluxBalanceAnalysisAdapter::fba()
{
  FluxBalanceSolution* fbs = new FluxBalanceSolution(lp_->get_cols());
  double obj = lp_->solve();
  fbs->obj = obj;

  //fbs.flux_vector = new vector<double>(lp_->get_cols());
  
  for (int i = 0; i < lp_->get_cols(); i++)
  {
    fbs->flux_vector->insert(fbs->flux_vector->begin() + i, lp_->get_col_prim(i));
  }
  return fbs;
}

FluxBalanceSolution::FluxBalanceSolution(int size)
{
  this->flux_vector = new vector<double>(size);
}

FluxBalanceSolution::~FluxBalanceSolution()
{
  delete this->flux_vector;
}

double FluxBalanceSolution::flux_sum()
{
  double acc = 0.0;
  for (auto e : *flux_vector)
  {
    acc += abs(e);
  }
  return acc;
}

FluxBalanceSolution* FluxBalanceAnalysisAdapter::pfba(int index)
{
  
  double pivot = lp_->solve();
  cout << "pivot: " << pivot << endl;
  lp_->write_lp("D:/before.txt");
  //minimize all reaction flux
  //WARNING THIS WILL MAXIMIZE NEGATIVE FLUX !!!!!! (Direction !?)
  lp_->set_obj_dir(Op::MIN);
  for (int i = 0; i < lp_->get_cols(); i++)
  {
    lp_->set_obj_coef(i, 1);
  }
  //fix previous objective value
  lp_->set_col_bound(pivot, Op::GE, index, Op::LE, pivot);
  double obj = lp_->solve();
  lp_->write_lp("D:/after.txt");
  cout << "obj: " << obj << endl;
  FluxBalanceSolution* fbs = new FluxBalanceSolution(lp_->get_cols());
  fbs->obj = obj;
  for (int i = 0; i < lp_->get_cols(); i++)
  {
    fbs->flux_vector->insert(fbs->flux_vector->begin() + i, lp_->get_col_prim(i));
  }
  return fbs;
}

