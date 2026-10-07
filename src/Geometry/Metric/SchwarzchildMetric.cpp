#include "SchwarzchildMetric.hpp"

namespace Geo 
{

  SchwarzchildMetric::SchwarzchildMetric(float M) 
  : Metric(), M(M) {};

  PhaseSpaceDerivatives SchwarzchildMetric::evaluate(const Point4& x, const Vec4& p) const
  {
    double r     = std::max(static_cast<double>(x.r), static_cast<double>(2.f * M + 1e-5f));
    double theta = std::clamp(static_cast<double>(x.theta), 1e-6, M_PI - 1e-6);

    double r2 = r * r;
    double rm2M = r - 2. * M;
    double sin0 = std::sin(theta);
    double cos0 = std::cos(theta);
    double sin20 = std::max(sin0 * sin0, 1e-12);

    double gtt = -r / rm2M;
    double grr = rm2M / r;
    double gthth = 1. / r2;
    double gphph = 1. / (r2 * sin20);

    Vec4 dxdl;
    dxdl.t     = static_cast<float>(gtt * p.t);
    dxdl.r     = static_cast<float>(grr * p.r);
    dxdl.theta = static_cast<float>(gthth * p.theta);
    dxdl.phi   = static_cast<float>(gphph * p.phi);

    double dgttdr   = (2. * M) / (rm2M * rm2M);
    double dgrrdr   = (2. * M) / r2;
    double dgththdr = -2. / (r2 * r);
    double dgphphdr = -2. / (r2 * r * sin20);

    double dgphphdtheta = -2. * cos0 / (r2 * sin20 * sin0);

    Vec4 dpdl(0.f);

    dpdl.r = static_cast<float>(-0.5 * (
        dgttdr   * p.t * p.t +
        dgrrdr   * p.r * p.r +
        dgththdr * p.theta * p.theta +
        dgphphdr * p.phi * p.phi
    ));

    dpdl.theta = static_cast<float>(-0.5 * (dgphphdtheta * p.phi * p.phi));

    return {dxdl, dpdl};
  }
  
  float SchwarzchildMetric::eventHorizonRadius() const 
  {
    return 2.f * M;
  }

  bool SchwarzchildMetric::isInsideHorizon(const Point4& x) const
  {
    return x.r <= eventHorizonRadius();
  }

  float SchwarzchildMetric::getMass() const
  {
    return M;
  }

  Mat4 SchwarzchildMetric::g(const Point4& x) const 
  {
    float r = std::max(x.r, 2.f * M + 1e-5f);
    float theta = std::clamp(x.theta, 1e-6f, static_cast<float>(M_PI) - 1e-6f);
    float sin0 = std::sin(theta);

    float gtt = -(1.f - 2.f * M / r);
    float grr = 1.f / (1.f - 2.f * M / r);
    float gthth = r * r;
    float gphph = r * r * sin0 * sin0;

    Mat4 mat(0.f);

    mat[0][0] = gtt;
    mat[1][1] = grr;
    mat[2][2] = gthth;
    mat[3][3] = gphph;

    return mat;
  }

  Point3 SchwarzchildMetric::toCartesian(const Point4& x) const
  {
    float r = x.r;
    float theta = x.theta;
    float phi = x.phi;
 
    float sinTheta = std::sin(theta);

    return Point3(
        r * sinTheta * std::cos(phi),
        r * sinTheta * std::sin(phi),
        r * std::cos(theta)
    );

  }
} // namespace Geo
