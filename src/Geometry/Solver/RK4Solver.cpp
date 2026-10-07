#include "RK4Solver.hpp"
#include <Geometry/Rays/GeodesicRay.hpp>
#include <Geometry/Solver/Solver.hpp>

namespace Geo 
{

  RK4Solver::RK4Solver() 
  : GeodesicSolver() {};

  StepResult RK4Solver::step(const GeodesicRay& ray, const Metric& metric, float dll) const
  {
    double dl = double(dll);

    Point4d x = Point4d(ray.x);
    Vec4d p = Vec4d(ray.p);

    PhaseSpaceDerivatives k1f = metric.evaluate(Point4(x), Vec4(p));
    Vec4d k1_dxdl = Vec4d(k1f.dxdl);
    Vec4d k1_dpdl = Vec4d(k1f.dpdl);

    Point4d x2 = x + 0.5 * dl * k1_dxdl;
    Vec4d   p2 = p + 0.5 * dl * k1_dpdl;
    PhaseSpaceDerivatives k2f = metric.evaluate(Point4(x2), Vec4(p2));
    Vec4d k2_dxdl = Vec4d(k2f.dxdl);
    Vec4d k2_dpdl = Vec4d(k2f.dpdl);

    Point4d x3 = x + 0.5 * dl * k2_dxdl;
    Vec4d   p3 = p + 0.5 * dl * k2_dpdl;
    PhaseSpaceDerivatives k3f = metric.evaluate(Point4(x3), Vec4(p3));
    Vec4d k3_dxdl = Vec4d(k3f.dxdl);
    Vec4d k3_dpdl = Vec4d(k3f.dpdl);

    Point4d x4 = x + dl * k3_dxdl;
    Vec4d   p4 = p + dl * k3_dpdl;
    PhaseSpaceDerivatives k4f = metric.evaluate(Point4(x4), Vec4(p4));
    Vec4d k4_dxdl = Vec4d(k4f.dxdl);
    Vec4d k4_dpdl = Vec4d(k4f.dpdl);

    Point4d nextXd = x + (dl / 6.) * (k1_dxdl + 2. * k2_dxdl + 2. * k3_dxdl + k4_dxdl);
    Vec4d   nextPd = p + (dl / 6.) * (k1_dpdl + 2. * k2_dpdl + 2. * k3_dpdl + k4_dpdl);

    double theta   = nextXd.theta;
    double phi     = nextXd.phi;
    double p_theta = nextPd.theta;

    while (theta < 0. || theta > M_PI) 
    {
        if (theta < 0.) 
        {
            theta = -theta;
            phi += M_PI;
            p_theta = -p_theta;
        } 
        else if (theta > M_PI) 
        {
            theta = 2. * M_PI - theta;
            phi += M_PI;
            p_theta = -p_theta;
        }
    }

    nextXd.theta = theta;
    nextXd.phi = phi; 
    nextPd.theta = p_theta;

    Point4 nextX = Point4(nextXd);
    Vec4   nextP = Vec4(nextPd);

    float nextLambda = ray.lambda + dl;

    float r = static_cast<float>(nextXd.r);
    float recommendedNextStep = std::max(1e-4f, 0.01f * r);


    return { GeodesicRay(nextX, nextP, nextLambda, ray.Q), recommendedNextStep };
  }

  
  

} // namespace Geo
