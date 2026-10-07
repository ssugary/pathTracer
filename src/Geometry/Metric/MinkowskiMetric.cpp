#include "MinkowskiMetric.hpp"

namespace Geo 
{

  MinkowskiMetric::MinkowskiMetric()
  : Metric() {};

  PhaseSpaceDerivatives MinkowskiMetric::evaluate(const Point4&, const Vec4& p) const
  {
    return {{-p.t, p.r, p.theta, p.phi}, Vec4(0.f)};
  }
  
  float MinkowskiMetric::eventHorizonRadius() const
  {
    return 0.f;
  }
  
  bool MinkowskiMetric::isInsideHorizon(const Point4& x) const
  {
    return false;
  }

  float MinkowskiMetric::getMass() const
  {
    return 1.f;
  }

  Mat4 MinkowskiMetric::g(const Point4&) const 
  {
    Mat4 id;
    id[0][0] = -1;
    return id;
  }

  Point3 MinkowskiMetric::toCartesian(const Point4& x) const
  {
    return Point3(x.r, x.theta, x.phi);
  }

} // namespace Geo
