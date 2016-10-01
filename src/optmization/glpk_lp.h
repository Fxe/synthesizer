#ifndef SYNTHESIZER_OPTIMIZATION_GLPK_LP_H_
#define SYNTHESIZER_OPTIMIZATION_GLPK_LP_H_

#include <map>
#include <set>
#include "math/operator.h"
#include "blas/matrix.h"
#include "glpk.h"

using namespace std;
using namespace math;

using blas::Matrix;

namespace optimization
{

class GlpkLp
{
  public:
    GlpkLp(int rows, int cols);
    ~GlpkLp();

    int get_rows() const;
    int get_cols() const;

    void set_name(string name);

    //aliases
    void set_row_alias(int row, const string name);
    void set_col_alias(int col, const string name);
    int get_row_index(const string alias) const;
    int get_col_index(const string alias) const;

    void set_obj_coef(int var, double coef);
    void set_obj_dir(Op op);
    //set row bounds
    void set_row_bound(int var, Op op, double value);
    //set col bounds
    void set_col_bound(double lb, Op op_lb, int var, Op op_ub, double ub);
    //set matrix
    void set_matrix(Matrix *matrix);
    void set_matrix_value(int row, int col, double value);
    void load_matrix();
    //set obj dir
    double solve();
    double solve_exact();
    double get_col_prim(int var);
    //io
    int write_lp(string path);
    glp_prob* get_lp();
    void after_solve();

  private:
    map<string, int>* row_alias_map_ = new map<string, int>();
    map<string, int>* col_alias_map_ = new map<string, int>();
    int rows;
    int cols;
    //glpk matrix
    int* ia;
    int* ja;
    double* ar;

    glp_prob *lp;

    int to_glp_bound_type(Op op);
    
};

} // namespace optimization 

#endif // SYNTHESIZER_OPTIMIZATION_GLPK_LP
