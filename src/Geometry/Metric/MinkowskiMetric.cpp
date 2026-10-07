#include "MinkowskiMetric.hpp"

namespace Geo 
{

  MinkowskiMetric::MinkowskiMetric()
  : Metric() {};

  PhaseSpaceDerivatives MinkowskiMetric::evaluate(const Point4&, const Vec4& p) const
  {
    return {p, Vec4(0.f)};
  }
  
  float MinkowskiMetric::eventHorizonRadius() const
  {
    return 0.f;
  }
  
  bool MinkowskiMetric::isInsideHorizon(const Point4& x) const
  {
    return x.y <= eventHorizonRadius();
  }

  float MinkowskiMetric::getMass() const
  {
    return 1.f;
  }

} // namespace Geo
