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

};

pgraph::OperatingUnit::OperatingUnit(int* a, int* b)
{
  a_ = new set < int > ;
  b_ = new set < int > ;
  std::cout << a << " " << b << std::endl;
};

pgraph::OperatingUnit::OperatingUnit(set<int>* a, set<int>* b) {
  a_ = a;
  b_ = b;
  cout << a->size() << " - " << b->size() << endl;
};

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