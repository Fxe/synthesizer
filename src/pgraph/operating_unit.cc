#include "operating_unit.h"
#include <iostream>
#include <ostream>
/*
template class pgraph::OperatingUnit<int>;
template class pgraph::OperatingUnit<short>;
*/

using namespace std;

pgraph::OperatingUnit::OperatingUnit()
{
  int id_ = 0;
};

pgraph::OperatingUnit::OperatingUnit(int id, int* a, int* b)
{
  id_ = id;
  a_ = new set < int > ;
  b_ = new set < int > ;
  std::cout << a << " " << b << std::endl;
};

pgraph::OperatingUnit::OperatingUnit(int id, set<int>* a, set<int>* b)
{
  id_ = id;
  a_ = a;
  b_ = b;
  cout << a->size() << " - " << b->size() << endl;
};

int pgraph::OperatingUnit::getId() { return id_; };

int pgraph::OperatingUnit::size()
{ 
  return a_->size() + b_->size();
};

set<int>* pgraph::OperatingUnit::get_a() { return a_; };
set<int>* pgraph::OperatingUnit::get_b() { return b_; };

/*
ostream& operator<< (ostream &strm, pgraph::OperatingUnit &ou) {
  return strm << "(" << ")";
  }*/