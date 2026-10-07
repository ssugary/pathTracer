#include "KerrMetric.hpp"
#include <Utils/common.hpp>

namespace Geo 
{

  KerrMetric::KerrMetric(float M, float a)
  : Metric(), M(M), a(std::clamp(a, 0.f, 0.9999f * M)) {};

  PhaseSpaceDerivatives KerrMetric::evaluate(const Point4& x, const Vec4& p) const 
  {
    float r = x.y;
    float r2 = r * r;
    float a2 = a * a;
    float r2a2 = r2 + a2;
    float theta = std::clamp(x.z, SHADOW_EPSILON, PI - SHADOW_EPSILON);

    float sin0 = std::sin(theta);
    float cos0 = std::cos(theta);
    float sin20 = sin0 * sin0;
    float cos20 = cos0 * cos0;

    float sigma = r2 + a2 * cos20;
    float delta = r2 - 2.f * M * r + a2;
    float sigmadelta = sigma * delta;
    float sigma2 = sigma * sigma;
    float gtt = std::pow((r2a2), 2.f) - a2 * delta * sin20; 
    gtt *= -1.f/(delta * sigma);

    float grr = delta / sigma;
    float g00 = 1.f/sigma;
    float gpp = delta - a2 * sin20;
    gpp /= sigmadelta * sin20;
    float gtp = - 2.f * M * r * a;
    gtp /= sigmadelta;

    Vec4 dxdl(0.f);

    dxdl.x = gtt * p.x + gtp * p.w;
    dxdl.y = grr * p.y;
    dxdl.z = g00 * p.z;
    dxdl.w = gtp * p.x + gpp * p.w;

    float dsigmadr = 2.f * r;
    float ddeltadr = 2.f * r - 2.f * M;
    float dsigmad0 = -2.f * a2 * sin0 * cos0;

    float dgttdr   = -((4.f * r * (r2a2) - a2 * ddeltadr * sin20) * (sigmadelta) - 
    (std::pow(r2a2, 2.f) - a2 * delta * sin20) * (dsigmadr * delta + sigma * ddeltadr)) 
    / std::pow(sigmadelta, 2.f);

    float dgrrdr   = (ddeltadr * sigma - delta * dsigmadr) / sigma2;
    float dgththdr = -dsigmadr / sigma2;

    float dgphphdr = ((ddeltadr) * (sigmadelta * sin20) - 
        (delta - a2 * sin20)* sin20 * (dsigmadr * delta + sigma * ddeltadr))
        / std::pow(sigmadelta * sin20, 2.f);

    float dgtphdr  = -((2.f * M * a) * (sigmadelta) - (2.f * M * r * a) 
        * (dsigmadr * delta + sigma * ddeltadr)) / (std::pow(sigmadelta, 2.f));

    Vec4 dpdl(0.f);

    dpdl.y = -0.5f * (
             dgttdr   * p.x * p.x +
             dgrrdr   * p.y * p.y +
             dgththdr * p.z * p.z +
             dgphphdr * p.w * p.w +
             2.f * dgtphdr * p.x * p.w
             );
  
    float dgttdtheta   = -(-a2 * delta * (2.0f * sin0 * cos0) * (sigmadelta) - 
        (std::pow(r2a2, 2.f) - a2 * delta * sin20) * (dsigmad0 * delta)) / 
        (std::pow(sigmadelta, 2.f));

    float dgrrdtheta   = -(delta * dsigmad0) / sigma2;
    float dgththdtheta = -dsigmad0 / sigma2;

    float dnumphph     = -2.f * a2 * sin0 * cos0;
    float ddenphph     = dsigmad0 * delta * sin20 + sigmadelta * (2.f * sin0 * cos0);
    float dgphphdtheta = (dnumphph * (sigmadelta * sin20) - (delta - a2 * sin20) * ddenphph) / 
      (std::pow(sigmadelta * sin20, 2.f));

    float dgtphdtheta  = -(-(2.f * M * r * a) * (dsigmad0 * delta)) / std::pow(sigmadelta, 2.f);

    dpdl.z = -0.5f * (
             dgttdtheta   * p.x * p.x +
             dgrrdtheta   * p.y * p.y +
             dgththdtheta * p.z * p.z +
             dgphphdtheta * p.w * p.w +
             2.f * dgtphdtheta * p.x * p.w
             );

    return { dxdl, dpdl };
  }
  float KerrMetric::eventHorizonRadius() const
  {
    return M + std::sqrt(std::max(M * M - a * a, 0.f));
  }
  bool KerrMetric::isInsideHorizon(const Point4& x) const
  {
    return x.y <= eventHorizonRadius();
  }
  float KerrMetric::getSpin() const
  {
    return a;
  }
  float KerrMetric::getMass() const
  {
    return M;
  }

} // namespace Geo
