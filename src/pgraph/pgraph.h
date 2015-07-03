#ifndef SYNTHESIZER_PGRAPH_PGRAPH_H_
#define SYNTHESIZER_PGRAPH_PGRAPH_H_

#include "operating_unit.h"
#include <string>
#include <vector>
#include <map>

namespace pgraph {
class ProcessGraph {
  public:
    ProcessGraph(vector<OperatingUnit> *o);
    OperatingUnit& getOperatingUnit(int id);
    map<int, OperatingUnit>& getOperatingUnitMap();
    string getMaterialAlias(int m);
    void setMaterialAlias(int m, string alias);
  private:
    map<int, string> *material_alias;
    map<int, string> *operating_unit_alias;
    map<int, OperatingUnit> *o_;
};
} // namespace pgraph

#endif // SYNTHESIZER_PGRAPH_PGRAPH
