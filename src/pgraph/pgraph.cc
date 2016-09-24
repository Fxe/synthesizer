#include "pgraph.h"
#include <iostream>

pgraph::ProcessGraph::ProcessGraph(vector<OperatingUnit> *o)
{
  material_alias = new map < int, string > ;
  operating_unit_alias = new map < int, string >;
  o_ = new map < int, OperatingUnit > ;
  for (OperatingUnit os : *o) {
    cout << "added ou" << endl;
    (*o_)[os.getId()] = os;
  }
  cout << "ou: " << o_->size() << endl;
};

string pgraph::ProcessGraph::getMaterialAlias(int m) {
  map<int, string>::iterator it = material_alias->find(m);
  if (it == material_alias->end())
  {
    return to_string(m);
  }
  else
  {
    return it->second;
  }
}

void pgraph::ProcessGraph::setMaterialAlias(int m, string alias) {
  material_alias->insert(pair<int, string>(m, alias));
}

pgraph::OperatingUnit* pgraph::ProcessGraph::getOperatingUnit(int id)
{
  map<int, OperatingUnit>::iterator it = o_->find(id);
  if (it == o_->end())
  {
    OperatingUnit *o = 0;
    return o;
  }
  else
  {
    OperatingUnit ou = it->second;
    return &ou;
  }
}

map<int, pgraph::OperatingUnit>* pgraph::ProcessGraph::getOperatingUnitMap()
{
  return o_;
}

pgraph::ProcessGraph* pgraph::ProcessGraphFactory::make()
{
  return NULL;
}