#include "SchwarzchildMetric.hpp"

namespace Geo 
{

  SchwarzchildMetric::SchwarzchildMetric(float M) 
  : Metric(), M(M) {};

  PhaseSpaceDerivatives SchwarzchildMetric::evaluate(const Point4& x, const Vec4& p) const
  {
    float r = x.y;
    float theta = x.z;

    float r2 = r * r;
    float rm2M = r - 2.f * M;
    float sin0 = std::sin(theta);
    float cos0 = std::cos(theta);

    Vec4 dxdl = p;

    Vec4 dpdl;

    dpdl.x = -(2.f * M / (r * rm2M)) * p.x * p.y;

    dpdl.y = -(M * rm2M / (r2 * r)) * (p.x * p.x)
                  + (M / (r * rm2M)) * (p.y * p.y)
                  + rm2M * (p.z * p.z)
                  + rm2M * sin0 * sin0 * (p.w * p.w);

    dpdl.z = -(2.f / r) * p.y * p.z 
                  + sin0 * cos0 * (p.w * p.w);

    dpdl.w = -(2.f / r) * p.y * p.w 
                  -(2.f * cos0 / sin0) * p.z * p.w;

    return {dxdl, dpdl};

  }
  
  float SchwarzchildMetric::eventHorizonRadius() const 
  {
    return 2.f * M;
  }

  bool SchwarzchildMetric::isInsideHorizon(const Point4& x) const
  {
    return x.y <= eventHorizonRadius();
  }

  float SchwarzchildMetric::getMass() const
  {
    return M;
  }
} // namespace Geo
