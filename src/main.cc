#include "pgraph/operating_unit.h"
#include "pgraph/pgraph.h"

#include <iostream>

int main(int argc, char** argv)
{
  int a[] = {1, 2};
  int b[] = {3, 4};
  pgraph::OperatingUnit o1;
  pgraph::OperatingUnit o2 (a, b);
  std::cout << argc << argv << std::endl;
  return 0;
}
