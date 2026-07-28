#include "TriangleFilter.hpp"
#include <algorithm>

namespace Fil 
{
  float TriangleFilter::evaluate(const Point2& p) const 
  {
    return std::max(0.f, radius[0] - std::abs(p[0])) *
           std::max(0.f, radius[1] - std::abs(p[1])); 
  }
}
