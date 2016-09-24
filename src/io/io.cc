#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "io.h"

model::StoichiometricModel* io::ReadMatrixFile2(const string path)
{
  Matrix* matrix = 0;
  vector<int> rev;
  vector<double> lb, ub;
  vector<string> cpd_alias, rxn_alias;
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
    int rows = atoi(word.c_str());
    iss >> word;
    int cols = atoi(word.c_str());
    matrix = new Matrix(rows, cols);
    //cout << rows << "x" << cols << endl;
    //read reaction line
    getline(fs, line, '\n');
    iss.clear();
    iss.str(line);
    //cout << line << endl;
    //skip first column
    getline(iss, line, '\t');
    std::cout << "read rxn alias" << endl;
    while (!iss.eof())
    {
      iss >> word;
      rxn_alias.push_back(word);
    }
    
    std::cout << "read matrix alias" << endl;
    for (int i = 0; i < rows; i++)
    {
      //read a single metabolite row
      getline(fs, line, '\n');
      iss.clear();
      iss.str(line);

      //first column is the metabolite alias
      getline(iss, line, '\t');
      cpd_alias.push_back(line);
      //remaing are matrix values
      for (int j = 0; j < cols; j++)
      {
        getline(iss, line, '\t');
        const double value = stod(line.c_str());
        matrix->set_value(i, j, value);
      }
    }

    //read lb
    getline(fs, line, '\n');
    iss.clear();
    iss.str(line);
    for (int i = 0; i < cols; i++)
    {
      getline(iss, word, '\t');
      const double value = stod(word.c_str());
      lb.push_back(value);
    }
    //read ub
    getline(fs, line, '\n');
    iss.clear();
    iss.str(line);
    for (int i = 0; i < cols; i++)
    {
      getline(iss, word, '\t');
      const double value = stod(word.c_str());
      ub.push_back(value);
    }
    while (getline(fs, line, '\n'))
    {
      cout << "REST: " << line << endl;
    }
  }
  fs.close();

  model::StoichiometricModel* sm = new model::StoichiometricModel(
    matrix, &lb, &ub, &cpd_alias, &rxn_alias);

  return sm;
}

void io::ReadMatrixFile(const string path, Matrix* matrix, vector<int> rev,
                        vector<double>* lb, vector<double>* ub,
                        vector<string>* cpd_alias, vector<string>* rxn_alias)
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
    int rows = atoi(word.c_str());
    iss >> word;
    int cols = atoi(word.c_str());
    matrix = new Matrix(rows, cols);
    //cout << *rows << "x" << *cols << endl;
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

    for (int i = 0; i < rows; i++)
    {
      //read a single metabolite row
      getline(fs, line, '\n');
      iss.clear();
      iss.str(line);

      //first column is the metabolite alias
      getline(iss, line, '\t');
      cpd_alias->push_back(line);
      //remaing are matrix values
      for (int j = 0; j < cols; j++)
      {
        getline(iss, line, '\t');
        const double value = stod(line.c_str());
        matrix->set_value(i, j, value);
      }
    }
    lb = new vector<double>(cols);
    ub = new vector<double>(cols);

    //read lb
    getline(fs, line, '\n');
    iss.clear();
    iss.str(line);
    for (int i = 0; i < cols; i++)
    {
      getline(iss, word, '\t');
      const double value = stod(word.c_str());
      lb->push_back(value);
    }
    //read ub
    getline(fs, line, '\n');
    iss.clear();
    iss.str(line);
    for (int i = 0; i < cols; i++)
    {
      getline(iss, word, '\t');
      const double value = stod(word.c_str());
      ub->push_back(value);
    }
    while (getline(fs, line, '\n'))
    {
      cout << "REST: " << line << endl;
    }
  }
  fs.close();
}

void io::read_matrix_file(const string path,
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

  //cout << path.c_str() << endl;
}

void io::WriteFile(string str, string path)
{
  
  std::ofstream fs;
  fs.open(path.c_str(), ios::out);
  fs << str;
  fs.close();
  
}