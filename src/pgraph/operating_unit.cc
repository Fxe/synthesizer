#include "operating_unit.h"
#include <iostream>
/*
template class pgraph::OperatingUnit<int>;
template class pgraph::OperatingUnit<short>;
*/
pgraph::OperatingUnit::OperatingUnit()
{

};

pgraph::OperatingUnit::OperatingUnit(int* a, int* b)
{
  std::cout << a << " " << b << std::endl;
};
