#ifndef SYNTHESIZER_PGRAPH_OPERATING_UNIT_H_
#define SYNTHESIZER_PGRAPH_OPERATING_UNIT_H_

#include <set>
#include <ostream>

using namespace std;

namespace pgraph {

//template<class T>
class OperatingUnit {
    
  public:
    OperatingUnit();
    OperatingUnit(int id, int* a, int* b);
    OperatingUnit(int id, set<int>* a, set<int>* b);
    set<int>* get_a();
    set<int>* get_b();
    int size();
    int getId();
  private:
    int id_;
    set<int> *a_, *b_;
    friend ostream& operator<<(ostream&, OperatingUnit&);
};

} // namespace pgraph

#endif // SYNTHESIZER_PGRAPH_OPERATING_UNIT_H_
