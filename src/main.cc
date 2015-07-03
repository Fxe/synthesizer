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

int main2(int argc, char** argv)
{
  std::cout << argc << " " << argv << std::endl;
  int a[] = {1, 2};
  int b[] = {3, 4};

  pgraph::ProcessGraph* pgraph = test::MakeTestPgraph1();
  //iterator<pgraph::OperatingUnit> it = O.begin();
  

  //cout << o2 << endl;
  set<int> r = { 5, 7, 10, 11, 12 };

  pgraph::OperatingUnit find = pgraph->getOperatingUnit(2);
  cout << &find << endl;
  pgraph::SolutionStructureGeneration ssg(1, &r, pgraph);
  ssg.solve();

  
  set<int> set1;
  for (int i = 0; i < 5; i++) set1.insert(i);
  pgraph::PowerSet(set1);
  set<int> set2;
  for (int i = 0; i < 7; i++) set2.insert(i);
  pgraph::PowerSet(set2);
  return 0;
}
