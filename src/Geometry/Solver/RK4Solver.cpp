#include "RK4Solver.hpp"
#include <Geometry/Rays/GeodesicRay.hpp>
#include <Geometry/Solver/Solver.hpp>

namespace Geo 
{

  RK4Solver::RK4Solver() 
  : GeodesicSolver() {};

  StepResult RK4Solver::step(const GeodesicRay& ray, std::shared_ptr<Metric> metric, float dl) const
  {
    PhaseSpaceDerivatives k1 = metric->evaluate(ray.x, ray.p);

    Point4 x2 = ray.x + 0.5f * dl * k1.dxdl;
    Vec4   p2 = ray.p + 0.5f * dl * k1.dpdl;
    PhaseSpaceDerivatives k2 = metric->evaluate(x2, p2);

    Point4 x3 = ray.x + 0.5f * dl * k2.dxdl;
    Vec4   p3 = ray.p + 0.5f * dl * k2.dpdl;
    PhaseSpaceDerivatives k3 = metric->evaluate(x3, p3);

    Point4 x4 = ray.x + dl * k3.dxdl;
    Vec4   p4 = ray.p + dl * k3.dpdl;
    PhaseSpaceDerivatives k4 = metric->evaluate(x4, p4);

    Point4 nextX = ray.x + (dl / 6.f) * (k1.dxdl + 2.f * k2.dxdl + 2.f * k3.dxdl + k4.dxdl);
    Vec4   nextP = ray.p + (dl / 6.f) * (k1.dpdl + 2.f * k2.dpdl + 2.f * k3.dpdl + k4.dpdl);

    return {GeodesicRay(nextX, nextP), 0.f};
  }

  
  

} // namespace Geo
