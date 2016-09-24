#include "pgraph/pgraph_utilities.h"
#include "pgraph/operating_unit.h"
#include "pgraph/pgraph.h"
#include "pgraph/ssg.h"

#include <iostream>
#include <ostream>
#include <vector>
#include <set>
#include "test/mock_data.h"
using namespace std;
using namespace pgraph;

int main(int argc, char** argv)
{
  
  //TESTING !
  std::cout << argc << " " << argv << std::endl;
  int a[] = {1, 2};
  int b[] = {3, 4};

  pgraph::ProcessGraph* pgraph = test::MakeTestPgraph1();
  //iterator<pgraph::OperatingUnit> it = O.begin();
  
  set<int> r = { 5, 7, 10, 11, 12 };
  pgraph::OperatingUnit* find = pgraph->getOperatingUnit(2);
  cout << find << " " << pgraph::set_string(r) << endl;

  set<set<int>> ps = pgraph::PowerSet(r);
  for (auto e : ps)
  {
    cout << pgraph::set_string(e) << endl;
  }

  int oua_sz = 7;
  OperatingUnit** oua = test::MakeTestOua1();
  for (int i = 0; i < oua_sz; i++)
  {
    //oua[i] = pgraph->getOperatingUnit(i);
    cout << oua[i]->getId() << " -> " << *oua[i] << endl;
  }

  map<int, set<int>*>* delta = buildDelta(oua, oua_sz);



  pgraph::SolutionStructureGeneration* ssg = new pgraph::SolutionStructureGeneration(1, &r, pgraph);
  cout << "solve !" << endl;
  pgraph->getOperatingUnitMap();
  //ssg->solve();
  
  int ou1_lhs[] = { 1 , 2 };
  int ou1_rhs[] = { 1 , 2 };
  pgraph::OperatingUnit* ou1 = new pgraph::OperatingUnit(1, ou1_lhs, 2, ou1_rhs, 2);
  pgraph::OperatingUnit* ou2 = new pgraph::OperatingUnit();

  pgraph::OperatingUnit ou3(1, ou1_lhs, 2, ou1_rhs, 2);
  cout << ou3 << endl;
  for (auto e : *ou1->get_a())
  {
    cout << e;
  }
  cout << *ou1 << endl;
  //cout << *ou2 << endl;
  /*
  //cout << o2 << endl;
  


  pgraph::SolutionStructureGeneration ssg(1, &r, pgraph);
  ssg.solve();

  
  set<int> set1;
  for (int i = 0; i < 5; i++) set1.insert(i);
  pgraph::PowerSet(set1);
  set<int> set2;
  for (int i = 0; i < 7; i++) set2.insert(i);
  pgraph::PowerSet(set2);
  */
  return 0;
}
