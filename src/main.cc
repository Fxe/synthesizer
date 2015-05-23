#include "pgraph/pgraph_utilities.h"
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

  pgraph::OperatingUnit o1(1, &o1_l, &o1_r);
  pgraph::OperatingUnit o2(2, &o2_l, &o2_r);
  pgraph::OperatingUnit o3(3, &o3_l, &o3_r);
  pgraph::OperatingUnit o4(4, &o4_l, &o4_r);
  pgraph::OperatingUnit o5(5, &o5_l, &o5_r);
  pgraph::OperatingUnit o6(6, &o6_l, &o6_r);
  pgraph::OperatingUnit o7(7, &o7_l, &o7_r);

  vector<pgraph::OperatingUnit> O = { o1, o2, o3, o4, o5, o6, o7 };
  //iterator<pgraph::OperatingUnit> it = O.begin();
  std::cout << argc << " " << argv << std::endl;

  //cout << o2 << endl;
  set<int> r = { 5, 7, 10, 11, 12 };
  pgraph::ProcessGraph pgraph (&O);
  pgraph.setMaterialAlias(1, "A");
  pgraph.setMaterialAlias(2, "B");
  pgraph.setMaterialAlias(3, "C");
  pgraph.setMaterialAlias(4, "D");
  pgraph.setMaterialAlias(5, "E");
  pgraph.setMaterialAlias(6, "F");
  pgraph.setMaterialAlias(7, "G");
  pgraph.setMaterialAlias(8, "H");
  pgraph.setMaterialAlias(9, "I");
  pgraph.setMaterialAlias(10,"J");
  pgraph.setMaterialAlias(11,"K");
  pgraph.setMaterialAlias(12,"L");

  pgraph::OperatingUnit find = pgraph.getOperatingUnit(2);
  cout << &find << endl;
  pgraph::SolutionStructureGeneration ssg(1, &r, &pgraph);
  ssg.solve();

  
  set<int> set1;
  for (int i = 0; i < 5; i++) set1.insert(i);
  pgraph::PowerSet(set1);
  set<int> set2;
  for (int i = 0; i < 7; i++) set2.insert(i);
  pgraph::PowerSet(set2);
  return 0;
}
