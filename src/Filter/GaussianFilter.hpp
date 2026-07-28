#ifndef GAUSSIAN_FILTER_HPP
#define GAUSSIAN_FILTER_HPP

#include "Filter.hpp"
#include <cmath>

namespace Fil {

class GaussianFilter : public Filter
{
  private:
  
    const float alpha;
    const float expX;
    const float expY;

  public:

    GaussianFilter(const Vec2& radius, float alpha)
    : Filter(radius), alpha(alpha), 
      expX(std::exp(-alpha * radius[0] * radius[0])), 
      expY(std::exp(-alpha * radius[1] * radius[1]))
    {};
  
    float evaluate(const Point2& p) const override;
    float gaussian(float d, float expv) const;
};

} // namespace GaussianFilter

#endif // GAUSSIANFILTER_HPP
