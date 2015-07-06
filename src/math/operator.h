#ifndef SYNTHESIZER_MATH_OP_H_
#define SYNTHESIZER_MATH_OP_H_
#include <vector>

using std::vector;
using std::pair;

namespace math
{

enum class Op
{
  EQUAL = 0,
  GE = 1,
  LE = 2,
  UNBOUNDED = 100,
  MAX = 10,
  MIN = 11,
};

} //namespace math

#endif // SYNTHESIZER_MATH_OP