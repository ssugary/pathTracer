#ifndef TRIANGLE_FILTER_HPP
#define TRIANGLE_FILTER_HPP

#include "Filter.hpp"

namespace Fil {

  class TriangleFilter : public Filter
  {
    public:
      TriangleFilter(const Vec2& radius) : Filter(radius) {};
      float evaluate(const Point2& p) const override;      

  };

} // namespace TriangleFilter

#endif // TRIANGLEFILTER_HPP
