#ifndef SYNTHESIZER_OPTIMIZATION_GLPK_LP_H_
#define SYNTHESIZER_OPTIMIZATION_GLPK_LP_H_

#include <map>
#include <set>
#include "math/operator.h"
#include "glpk.h"

using namespace std;
using namespace math;

namespace optimization
{

class GlpkLp
{
  public:
    GlpkLp(int rows, int cols);
    ~GlpkLp();
    void set_name(string name);
    void set_row_alias(int row, string name);
    void set_col_alias(int col, string name);
    void set_obj_coef(int var, double coef);
    void set_obj_dir(Op op);
    //set row bounds
    void set_row_bound(int var, Op op, double value);
    //set col bounds
    void set_col_bound(double lb, Op op_lb, int var, Op op_ub, double ub);
    //set matrix
    void set_matrix();
    void set_matrix_value(int row, int col, double value);
    void load_matrix();
    //set obj dir
    double solve();
    double get_col_prim(int var);
    //io
    int write_lp(string path);
    glp_prob* get_lp();

  private:
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
