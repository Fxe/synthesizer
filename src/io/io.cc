#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "io.h"

using std::cout;
using std::getline;

vector<string> io::split(string str, char delimiter)
{
  vector<string> tokens;
  std::string token;
  std::istringstream iss(str);
  while (std::getline(iss, token, delimiter)) {
    tokens.push_back(token);
  }

  return tokens;
}

void io::ReadProblemFile(const string path, set<long>* substrates)
{
  std::cout << "ReadProblemFile " << path  << std::endl;
  std::fstream fs;
  string line;
  fs.open(path, ios::in);
  if (fs.is_open())
  {
    getline(fs, line, '\n');
    //cout << "HEADER " << line << endl;
    while (getline(fs, line, '\n'))
    {
      //cout << "REST: " << line << endl;
      std::istringstream iss(line);
      /*
      string target;
      iss >> target;
      string model;
      iss >> model;
      
      */
      //
      string target;
      std::getline(iss, target, '\t');
      string model;
      std::getline(iss, model, '\t');
      //std::cout << target << " >> " << model << std::endl;
      string subs;
      std::getline(iss, subs, '\t');
      string spis;
      std::getline(iss, spis, '\t');
      string smap;
      std::getline(iss, smap, '\t');
      for (auto e : split(subs, ' ')) {
        //std::cout << "\t" << e << std::endl;
        substrates->insert(atol(e.c_str()));
      }
      //std::cout << subs << std::endl;
    }
  }
  else {
    cout << ":(" << endl;
  }
  fs.close();
}

void io::ReadSolutionFile(const string path, set<string>* reactions, map<long, set<string>>* solutions, map<string, set<long>>* targetToSolutions)
{
  std::cout << "ReadSolutionFile" << path << std::endl;
  std::fstream fs;
  string line;
  string word;
  fs.open(path, ios::in);
  int l = 0;
  if (fs.is_open())
  {
    std::getline(fs, line, '\n');
    //cout << "HEADER " << line << endl;
    //std::istringstream iss(line);
    //iss >> word;
    //std::cout << word;
    l++;
    while (std::getline(fs, line, '\n'))
    {
      l++;
      if (!(l % 1000))
      {
        std::cout << "line: " << l << std::endl;
      }
      //cout << "REST: " << line << endl;
      std::istringstream iss(line);
      string target;
      iss >> target;
      if (targetToSolutions->find(target) == targetToSolutions->end()) {
        targetToSolutions->insert({target, set<long>()});
      }
      string model;
      iss >> model;
      string solId;
      iss >> solId;
      set<string> solution;
      while (!iss.eof())
      {
        iss >> word;
        //cout << "\t" << word << endl;
        solution.insert(word);
        reactions->insert(word);
      }
      long solId_ = atol(solId.c_str());
      solutions->insert({ solId_, solution });
      targetToSolutions->find(target)->second.insert(solId_);
      //cout << "-------------------------" << endl;
    }
  }
  else {
    std::cout << "error open file " << path << endl;
  }
  fs.close();
}

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
    std::getline(fs, line, '\n');
    std::istringstream iss(line);
    iss >> word;
    *rows = atoi(word.c_str());
    iss >> word;
    *cols = atoi(word.c_str());
    std::cout << *rows << "x" << *cols << endl;
    //read reaction line
    std::getline(fs, line, '\n');
    iss.clear();
    iss.str(line);
    std::cout << line << endl;
    //skip first column
    std::getline(iss, line, '\t');
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

void io::WriteNetMap(map<map<long, double>, set<long>> netMap, string path)
{
  std::fstream fs;
  fs.open(path, ios::out);
  std::cout << path << std::endl;
  if (fs.is_open())
  {
    fs << "net\tsolutions";
    for (auto e : netMap)
    {
      map<long, double> s = e.first;
      fs << "\n" << stoichToString(s) << "\t" << toString(e.second);
    }
    fs.close();
  }
  else
  {
    std::cerr << "error open file (write) " << path << std::endl;
  }
  
}

string io::stoichToString(map<long, double> stoich)
{
  std::ostringstream ss;
  for (auto e : stoich)
  {
    double v = e.second;
    if (v < 0.0)
    {
      ss << v << " " << e.first << " ";
    }
    
    //double v = e,
    //if ()
    //ss << e << " ";
  }
  ss << "<=>";
  for (auto e : stoich)
  {
    double v = e.second;
    if (v > 0.0)
    {
      ss << " " << v << " " << e.first;
    }
  }

  return ss.str();
}
