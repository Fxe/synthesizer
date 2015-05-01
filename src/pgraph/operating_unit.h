#ifndef SYNTHESIZER_PGRAPH_OPERATING_UNIT_H_
#define SYNTHESIZER_PGRAPH_OPERATING_UNIT_H_

#include <set>

namespace pgraph {

//template<class T>
class OperatingUnit {
    std::set<int> a, b;
  public:
    OperatingUnit();
    OperatingUnit(int* a, int* b);
private:

};
} // namespace pgraph

#endif // SYNTHESIZER_PGRAPH_OPERATING_UNIT_H_
