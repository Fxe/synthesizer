#include "ssg.h"
#include "pgraph_utilities.h"
#include <iostream>
#include <set>
#include <map>
#include <algorithm>
#include <string>

using namespace std;

pgraph::SolutionStructureGeneration::SolutionStructureGeneration(int p, set<int> *r, 
                                                                 pgraph::ProcessGraph* pg)
{
  pg_ = pg;
  p_ = new set < int > ;
  p_->insert(p);
  r_ = r;
};

void pgraph::SolutionStructureGeneration::solve()
{
  initDeltaMap();
  
  cout << "I am solving ... " << endl;
  
  ssg(p_, new set<int>, new map<int, set<int>>);
};

void pgraph::SolutionStructureGeneration::ssg(set<int>* p, set<int>* m, 
                                              map<int, set<int>> *d_map)
{
  cout << "P: " << set_string(*p) << " M: " << set_string(*m) << " D: " << map_string(*d_map) << endl;
  if (p->empty())
  {
    cout << "Solution: " << map_string(*d_map) << endl;
    return;
  }

  int x = *p->begin();
  p->erase(x);
  cout << "not empty -> " << pg_->getMaterialAlias(x) << endl;

  set<int>* delta_ = delta(x);
  cout << "producers of " << pg_->getMaterialAlias(x) 
       << " " << set_string(*delta_) << endl;

  // power set
  set<set<int>> pset = PowerSet(*delta_);
  
  for (auto &e : pset)
  {
    if (test(e, m, d_map))
    {
      set<int> p_new(*p);
      
      for (auto &ou_id : e)
      {
        OperatingUnit ou = pg_->getOperatingUnit(ou_id);
        set<int> *ou_lhs = ou.get_a();
        for (auto &ee : *ou_lhs)
        {
          p_new.insert(ee);
        }
      }
      set<int> p_new_f;
      set_difference(p_new.begin(), p_new.end(), 
                     r_->begin(), r_->end(), 
                     inserter(p_new_f, p_new_f.end()));

      cout << set_string(p_new) << set_string(*r_) << set_string(p_new_f) << endl;

      set<int> m_new(*m);
      map<int, set<int>> d_map_new(*d_map);
      d_map_new[x] = e;
      m_new.insert(x);
      ssg(&p_new_f, &m_new, &d_map_new);
    }
  }
}

void pgraph::SolutionStructureGeneration::initDeltaMap()
{
  deltaMap_ = new map < int, set<int>* >;
  for (auto const &it : pg_->getOperatingUnitMap())
  {
    OperatingUnit ou = it.second;
    set<int>::iterator ou_rhs = ou.get_b()->begin();
    for (; ou_rhs != ou.get_b()->end(); ou_rhs++)
    {
      if (deltaMap_->find(*ou_rhs) == deltaMap_->end())
      {
        deltaMap_->insert(make_pair(*ou_rhs, new set<int>));
        deltaMap_->find(*ou_rhs)->second->insert(ou.getId());
      }
      else
      {
        deltaMap_->find(*ou_rhs)->second->insert(ou.getId());
      }
    }
  }
  string str = "{";
  for (auto &e : *deltaMap_)
  {
    str += to_string(e.first) + " : " + set_string(*e.second);
  }
  str += "}";
  cout << str << endl;
}

set<int>* pgraph::SolutionStructureGeneration::delta(int x)
{
  map <int, set<int>*>::iterator it = deltaMap_->find(x);
  if (it == deltaMap_->end())
  {
    return new set < int > ;
  }
  return it->second;
}

bool pgraph::SolutionStructureGeneration::test(set<int> c, set<int>* m, 
                                               map<int, set<int>> *d_map)
{
  cout << "Test " << set_string(c) << endl;
  if (c.empty()) return 0;
  for (int y : *m)
  {
    //c | !d(y) = 0 AND (D(x) \ c) | d(y) = 0

    cout << y << endl;
  }
  return 1;
}