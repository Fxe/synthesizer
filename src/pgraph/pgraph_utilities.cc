#include "pgraph_utilities.h"
#include <string>
#include <iostream>

using namespace std;

namespace pgraph
{
  

  map<int, set<int>*>* buildDelta(OperatingUnit** ouArray, int ou_sz)
  {
    map<int, set<int>*>* delta = new map<int, set<int>*>();
    for (int i = 0; i < ou_sz; i++)
    {
      OperatingUnit* ou = ouArray[i];
      int id = ou->getId();
      for (auto &p : *ou->get_b())
      {
        cout << "p: " << p << endl;;
        map<int, set<int>*>::iterator f = delta->find(p);
        if (f == delta->end())
        {
          cout << "Add key: " << p << endl;
          delta->insert(make_pair(p, new set<int>()));
          cout << "Add: " << p << " -> " << id << endl;
          delta->find(p)->second->insert(id);
        }
        else
        {
          cout << "Add: " << p << " -> " << id << endl;
          f->second->insert(id);
        }
        //if existes key "p"
        //if not
      }
    }

    return delta;
  }

  set<set<int>> PowerSet(set<int> s)
  {
    cout << "PowerSet: " << s.size() << endl;
    set<set<int>> ret;
    ret.emplace();
    for (auto&& e : s) {
      std::set<set<int>> rs;
      for (auto x : ret) {
        x.insert(e);
        rs.insert(x);
      }
      ret.insert(begin(rs), end(rs));
    }
    cout << "done: " << ret.size() << endl;
    return ret;
  }

  string set_string(set<int> s)
  {
    string str = "[";
    //set<int>::iterator it;
    //for (auto &e : s)
    //{
    //  str += to_string(e) + ", ";
    //}
    str += "]";
    return str;
  }

  string map_string(map<int, set<int>> m)
  {
    string str = "{";
    for (auto &e : m)
    {
      str += to_string(e.first) + " : " + set_string(e.second);
    }
    str += "}";
    return str;
  }

} // namespace pgraph 



