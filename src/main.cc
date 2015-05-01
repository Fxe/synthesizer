#include "pgraph/operating_unit.h"
#include "pgraph/pgraph.h"
#include "pgraph/ssg.h"

#include <iostream>
#include <ostream>
#include <vector>
#include <set>

using namespace std;

int main(int argc, char** argv)
{
  int a[] = {1, 2};
  int b[] = {3, 4};
  
  /* A B C D E F G H I J  K  L
   * 1 2 3 4 5 6 7 8 9 10 11 12
   */
  set<int> o1_l = { 3 }; set<int> o1_r = { 1, 6 };
  set<int> o2_l = { 4 }; set<int> o2_r = { 1, 2 };
  set<int> o3_l = { 5, 6 }; set<int> o3_r = { 3 };
  set<int> o4_l = { 6, 7 }; set<int> o4_r = { 3, 4 };
  set<int> o5_l = { 7, 8 }; set<int> o5_r = { 4 };
  set<int> o6_l = { 10 }; set<int> o6_r = { 6 };
  set<int> o7_l = { 11, 12 }; set<int> o7_r = { 8 };

  pgraph::OperatingUnit o1(&o1_l, &o1_r);
  pgraph::OperatingUnit o2(&o2_l, &o2_r);
  pgraph::OperatingUnit o3(&o3_l, &o3_r);
  pgraph::OperatingUnit o4(&o4_l, &o4_r);
  pgraph::OperatingUnit o5(&o5_l, &o5_r);
  pgraph::OperatingUnit o6(&o6_l, &o6_r);
  pgraph::OperatingUnit o7(&o7_l, &o7_r);

  vector<pgraph::OperatingUnit> O = { o1, o2, o3, o4, o5, o6, o7 };

  std::cout << argc << " " << argv << std::endl;

  for (pgraph::OperatingUnit o : O)
  {
    cout << o.size() << endl;
  }

  //cout << o2 << endl;
  set<int> r = { 1, 2, 3, 4 };
  pgraph::ProcessGraph pgraph;
  pgraph::SolutionStructureGeneration ssg(3, &r, &pgraph);
  ssg.solve();
  return 0;
}
