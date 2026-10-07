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
    float theta = std::clamp(x.theta, SHADOW_EPSILON, PI - SHADOW_EPSILON);

    float sin0 = std::sin(theta);
    float cos0 = std::cos(theta);
    float sin20 = sin0 * sin0;
    float cos20 = cos0 * cos0;

    float sigma = r2 + a2 * cos20;
    float delta = r2 - 2.f * M * r + a2;
    float sigmadelta = sigma * delta;
    float sigma2 = sigma * sigma;
    float gtt = r2a2 * r2a2 - a2 * delta * sin20; 
    gtt *= -1.f/(delta * sigma);

    float grr = delta / sigma;
    float g00 = 1.f/sigma;
    float gpp = delta - a2 * sin20;
    gpp /= sigmadelta * sin20;
    float gtp = - 2.f * M * r * a;
    gtp /= sigmadelta;

    Vec4 dxdl(0.f);

    dxdl.t = gtt * p.t + gtp * p.phi;
    dxdl.r = grr * p.r;
    dxdl.theta = g00 * p.theta;
    dxdl.phi = gtp * p.t + gpp * p.phi;

    float dsigmadr = 2.f * r;
    float ddeltadr = 2.f * r - 2.f * M;
    float dsigmad0 = -2.f * a2 * sin0 * cos0;

    float dgttdr   = -((4.f * r * (r2a2) - a2 * ddeltadr * sin20) * (sigmadelta) - 
    (r2a2 * r2a2 - a2 * delta * sin20) * (dsigmadr * delta + sigma * ddeltadr)) 
    / (sigmadelta * sigmadelta);

    float dgrrdr   = (ddeltadr * sigma - delta * dsigmadr) / sigma2;
    float dgththdr = -dsigmadr / sigma2;

    float dgphphdr = ((ddeltadr) * (sigmadelta * sin20) - 
        (delta - a2 * sin20)* sin20 * (dsigmadr * delta + sigma * ddeltadr))
        / std::pow(sigmadelta * sin20, 2.f);

    float dgtphdr  = -((2.f * M * a) * (sigmadelta) - (2.f * M * r * a) 
        * (dsigmadr * delta + sigma * ddeltadr)) / (sigmadelta * sigmadelta);

    Vec4 dpdl(0.f);

    dpdl.r = -0.5f * (
             dgttdr   * p.t * p.t +
             dgrrdr   * p.r * p.r +
             dgththdr * p.theta * p.theta +
             dgphphdr * p.phi * p.phi +
             2.f * dgtphdr * p.t * p.phi
             );
  
    float dgttdtheta   = -(-a2 * delta * (2.0f * sin0 * cos0) * (sigmadelta) - 
        (r2a2 * r2a2 - a2 * delta * sin20) * (dsigmad0 * delta)) / 
        (sigmadelta * sigmadelta);

    float dgrrdtheta   = -(delta * dsigmad0) / sigma2;
    float dgththdtheta = -dsigmad0 / sigma2;

    float dnumphph     = -2.f * a2 * sin0 * cos0;
    float ddenphph     = dsigmad0 * delta * sin20 + sigmadelta * (2.f * sin0 * cos0);
    float dgphphdtheta = (dnumphph * (sigmadelta * sin20) - (delta - a2 * sin20) * ddenphph) / 
      (std::pow(sigmadelta * sin20, 2.f));

    float dgtphdtheta  = -(-(2.f * M * r * a) * (dsigmad0 * delta)) / (sigmadelta * sigmadelta);

    dpdl.theta = -0.5f * (
             dgttdtheta   * p.t * p.t +
             dgrrdtheta   * p.r * p.r +
             dgththdtheta * p.theta * p.theta +
             dgphphdtheta * p.phi * p.phi +
             2.f * dgtphdtheta * p.t * p.phi
             );

    return { dxdl, dpdl };
  }
  float KerrMetric::eventHorizonRadius() const
  {
    return M + std::sqrt(std::max(M * M - a * a, 0.f));
  }
  bool KerrMetric::isInsideHorizon(const Point4& x) const
  {
    return x.r <= eventHorizonRadius();
  }
  float KerrMetric::getSpin() const
  {
    return a;
  }
  float KerrMetric::getMass() const
  {
    return M;
  }

  Mat4 KerrMetric::g(const Point4& x) const 
  {

    float r = x.r;
    float theta = std::clamp(x.theta, 1e-6f, PI - 1e-6f);

    float sin0 = std::sin(theta);
    float cos0 = std::cos(theta);
    float sin20 = sin0 * sin0;
    float cos20 = cos0 * cos0;

    float r2 = r * r;
    float a2 = a * a;
    float sigma = r2 + a2 * cos20;
    float delta = r2 - 2.f * M * r + a2;

    float gtt = -(1.f - (2.f * M * r) / sigma);
    float gtp = -(2.f * M * r * a * sin20) / sigma;
    float grr = sigma / delta;
    float gthth = sigma;
    float gphph = (r2 + a2 + (2.f * M * r * a2 * sin20) / sigma) * sin20;

    Mat4 mat(0.f);
    mat[0][0] = gtt;   
    mat[0][3] = gtp;
    mat[1][1] = grr;
    mat[2][2] = gthth;
    mat[3][0] = gtp;   
    mat[3][3] = gphph;

    return mat;
  }

  Point3 KerrMetric::toCartesian(const Point4& x) const
  {
    float r = x.r;
    float theta = x.theta;
    float phi = x.phi;
 
    float sinTheta = std::sin(theta);
    float rad = std::sqrt(r * r + a * a);

    return Point3(
        rad * sinTheta * std::cos(phi),
        rad * sinTheta * std::sin(phi),
        r * std::cos(theta)
    );
  }

} // namespace Geo
