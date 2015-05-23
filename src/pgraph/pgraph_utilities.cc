#include "pgraph_utilities.h"
#include <string>
#include <iostream>

using namespace std;

namespace pgraph
{

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
    for (auto &e : s)
    {
      str += to_string(e) + ", ";
    }
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



