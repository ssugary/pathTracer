#include "GaussianFilter.hpp"

namespace Fil 
{
  float GaussianFilter::evaluate(const Point2& p) const 
  {
    return gaussian(p[0], expX) * gaussian(p[1], expY);
  }

  float GaussianFilter::gaussian(float d, float expv) const
  {
    return std::max(0.f, std::exp(-alpha * d * d) - expv);
  }

}
