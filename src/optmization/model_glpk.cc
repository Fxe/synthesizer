#include "model_glpk.h"
#include <iostream>

using namespace std;
using namespace optimization;

GlpkModel::GlpkModel(int rows, int cols) : GlpkLp(rows, cols)
{
  
}

void GlpkModel::add_drain()
{
  cout << "GlpkModel - add_drain() not implemented" << endl;
}