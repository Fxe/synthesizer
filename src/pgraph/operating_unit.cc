#include "operating_unit.h"
#include "pgraph_utilities.h"
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

pgraph::OperatingUnit::OperatingUnit(int id, int* a, int sa, int* b, int sb)
{
  id_ = id;
  a_ = new set < int > ;
  b_ = new set < int > ;
  for (int i = 0; i < sa; i++)
  {
    a_->insert(a[i]);
  }
  for (int i = 0; i < sb; i++)
  {
    b_->insert(b[i]);
  }
  //std::cout << a << " " << b << std::endl;
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


ostream& pgraph::operator<< (ostream &strm, pgraph::OperatingUnit &ou) {
  //string a_str = set_string(*(ou.a_));
  //strm << *ou;
  
  //ou.b_->begin();
  //for (auto e : *ou.b_)
  //{
  //  strm << e;
  //}
  strm << "(<";
  if (ou.a_)
  {
    for (auto e : *ou.a_)
    {
      strm << e << ", ";
    }
    
  }
  strm << ">, <";
  if (ou.b_)
  {
    for (auto e : *ou.b_)
    {
      strm << e << ", ";
    }

  }
  strm << ">)";
  return strm;
}