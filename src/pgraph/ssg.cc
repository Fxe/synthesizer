#include "ssg.h"
#include <iostream>
#include <set>
#include <algorithm>

using namespace std;

pgraph::SolutionStructureGeneration::SolutionStructureGeneration(int p, set<int> *r, pgraph::ProcessGraph* g)
{
  p_ = new set < int > ;
  p_->insert(p);
  r_ = r;
};

void pgraph::SolutionStructureGeneration::solve()
{
  set<int>::iterator it;
  
  for (it = r_->begin(); it != r_->end(); it++) {
    cout << *it;
  }
  
  cout << "I am solving ... " << endl;
  
  ssg(p_, r_);
};

void pgraph::SolutionStructureGeneration::ssg(set<int>* p, set<int>* m)
{
  if (p->empty())
  {
    cout << "empty" << endl;
    return;
  }

  int x = *p->begin();
  p->erase(x);
  cout << "not empty -> " << x << endl;
  // power set
  for (;;)
  {
    if (test(1, m, 2)) {
      set<int> p_new(*p);
      ssg(&p_new, m);
    }
    break;
  }
}

void pgraph::SolutionStructureGeneration::delta(int x)
{

}

bool pgraph::SolutionStructureGeneration::test(int c, set<int>* m, int somedelta)
{
  cout << "Test" << endl;
  for (int y : *m)
  {
    cout << y << endl;
  }
  return 0;
}