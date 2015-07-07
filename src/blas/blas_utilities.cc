#include "blas_utilities.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#ifdef UNIX
#include <time.h>
#include <sys/time.h>
#elif WIN32
#include <Windows.h>
#endif

double wall_time()
{
#ifdef UNIX

#elif WIN32
  LARGE_INTEGER time, freq;
  if (!QueryPerformanceFrequency(&freq)){
    //  Handle error
    return 0;
  }
  if (!QueryPerformanceCounter(&time)){
    //  Handle error
    return 0;
  }
  return (double)time.QuadPart / freq.QuadPart;
#endif
}

double blas::WallTime()
{
  return wall_time();
}

double blas::CpuTime()
{
  return 0;
}

double* blas::dInitArray(const int s, const double value)
{
  double* v = new double[s];
  for (int i = 0; i < s; i++)
  {
    v[i] = value;
  }
  return v;
}

double** blas::dInitMatrix(const int r, const int c, const double value)
{
  double** m = new double*[r];
  // naive way ! non contiguos
  for (int i = 0; i < r; i++)
  {
    m[i] = dInitArray(c, value);
  }
  // better ! too lazy to implement
  //m[0] = new double[r * c];
  return m;
}

void blas::read_matrix_file(const string path,
                            int* rows, int* cols,
                            double*** matrix, int** rev,
                            double** ub, double **lb,
                            vector<string>* rxn_alias,
                            vector<string>* cpd_alias)
{
  std::fstream fs;
  string line;
  string word;
  fs.open(path, ios::in);
  //std::getline(file., line);
  if (fs.is_open())
  {
    //read rows cols
    getline(fs, line, '\n');
    std::istringstream iss(line);
    iss >> word;
    *rows = atoi(word.c_str());
    iss >> word;
    *cols = atoi(word.c_str());
    cout << *rows << "x" << *cols << endl;
    //read reaction line
    getline(fs, line, '\n');
    iss.clear();
    iss.str(line);
    cout << line << endl;
    //skip first column
    getline(iss, line, '\t');
    while (!iss.eof())
    {
      iss >> word;
      rxn_alias->push_back(word);
    }
    
    //alloc matrix
    *matrix = new double*[*rows];
    for (int i = 0; i < *rows; i++)
    {
      (*matrix)[i] = new double[*cols];
      for (int j = 0; j < *cols; j++)
      {
        (*matrix)[i][j] = -999999;
      }
    }

    for (int i = 0; i < *rows; i++)
    {
      //read a single metabolite row
      getline(fs, line, '\n');
      iss.clear();
      iss.str(line);
      
      //first column is the metabolite alias
      getline(iss, line, '\t');
      cpd_alias->push_back(line);
      //remaing are matrix values
      for (int j = 0; j < *cols; j++)
      {
        getline(iss, line, '\t');
        (*matrix)[i][j] = stod(line.c_str());
      }
    }
    *lb = new double[*cols];
    *ub = new double[*cols];
    //read lb
    getline(fs, line, '\n');
    iss.clear();
    iss.str(line);
    for (int i = 0; i < *cols; i++)
    {
      getline(iss, word, '\t');
      (*lb)[i] = stod(word.c_str());
    }
    //read ub
    getline(fs, line, '\n');
    iss.clear();
    iss.str(line);
    for (int i = 0; i < *cols; i++)
    {
      getline(iss, word, '\t');
      (*ub)[i] = stod(word.c_str());
    }
    while (getline(fs, line, '\n'))
    {
      cout << "REST: " << line << endl;
    }
  }
  fs.close();
  
  cout << path.c_str() << endl;
}