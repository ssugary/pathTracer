#pragma once

#ifndef BOX_FILTER_HPP
#define BOX_FILTER_HPP

#include "Filter/Filter.hpp"

namespace Fil {
    
  class BoxFilter : public Filter 
  {
    public:
      BoxFilter(const Vec2 &radius) : Filter(radius) {};

      float evaluate(const Point2 &p) const override;
  };

} // namespace Fil

#endif
