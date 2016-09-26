#ifndef SYNTHESIZER_IO_IO_H_
#define SYNTHESIZER_IO_IO_H_

#include <map>
#include <set>
#include <vector>
#include "blas/matrix.h"
#include "model/stoichiometric_model.h"

using namespace std;
using blas::Matrix;

namespace io
{

  vector<string> split(string str, char delimiter);
  void ReadProblemFile(const string path, set<long>* substrates);
  void ReadSolutionFile(const string path, set<string>* reactions, map<long, set<string>>* solutions, map<string, set<long>>* targetToSolutions);

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

  void WriteNetMap(map<map<long, double>, set<long>> netMap, string path);

  template<class T>
  string toString(set<T> set)
  {
    std::ostringstream ss;
    for (auto e : set)
    {
      ss << e << " ";
    }

    return ss.str();
  }

  //template<class T>
  string stoichToString(map<long, double> stoich);
} //namespace io

#endif // SYNTHESIZER_IO