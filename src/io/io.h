#ifndef SYNTHESIZER_IO_IO_H_
#define SYNTHESIZER_IO_IO_H_

#include <vector>
#include "blas/matrix.h"
#include "model/stoichiometric_model.h"

using namespace std;
using blas::Matrix;

namespace io
{
  model::StoichiometricModel* ReadMatrixFile2(const string path);

  void ReadMatrixFile(const string path, Matrix* matrix, vector<int> rev,
                      vector<double>* lb, vector<double>* ub,
                      vector<string>* cpd_alias, vector<string>* rxn_alias);
  void read_matrix_file(const string path,
                        int* rows, int* cols,
                        double*** matrix, int** rev,
                        double** ub, double **lb,
                        vector<string>* rxn_alias,
                        vector<string>* cpd_alias);

  void WriteFile(string str, string path);
} //namespace io

#endif // SYNTHESIZER_IO