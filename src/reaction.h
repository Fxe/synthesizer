#include <vector>

using std::vector;

namespace bio
{

class Reaction
{
  public:
  private:
    int id_;
    vector<int> lhs_;
    vector<int> rhs_;
};

} //namespace bio

