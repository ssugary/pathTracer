#include "RelativisticIntegrator.hpp"
#include "Geometry/Solver/RK4Solver.hpp"

namespace Itg   
{

  RelativisticIntegrator::RelativisticIntegrator(std::shared_ptr<Cam::RelativisticCamera> cam, std::shared_ptr<Sam::Sampler> sampler, std::shared_ptr<Metric> metric, int maxSteps, float rEscape, float dLambda)
    : Integrator(), cam(std::move(cam)), sampler(std::move(sampler)), metric(std::move(metric)), maxSteps(maxSteps), rEscape(rEscape), dLambda(dLambda) {};   
 
  float RelativisticIntegrator::computeRedshift(const GeodesicRay& ray, float rDisk) const
  {
    float M = metric->getMass();
    float a = metric->getSpin();

    float omega = 1.f / (std::pow(rDisk, 1.5f) + a);
    float Eem = -(ray.p.x + omega * ray.p.w) * ::computeUt(rDisk, omega, M, a);

    return -ray.p.x / Eem;
  }

  std::optional<Color> RelativisticIntegrator::li(const GeodesicRay& gRay, const Scene& scene, Sam::Sampler& sampler) const
  {
    std::optional<Color> L;
    GeodesicRay ray = gRay;

    float rHorizon = metric->eventHorizonRadius();
    Point4 prevX = ray.x;

    for(int step{0}; step < maxSteps; ++step)
    {
      StepResult res = solver.step(ray, metric, dLambda);

      ray = res.ray;
      
      if(metric->isInsideHorizon(ray.x))
        return L;
      
      float r = ray.x.y;
      float theta = ray.x.z;
      
      if((prevX.z - PI_OVER_TWO) * (theta - PI_OVER_TWO) <= 0.f && r > rHorizon)
      {
        float rDisk = r;
        float g = computeRedshift(ray, rDisk);
      }

    }
  }

  void RelativisticIntegrator::render(const Scene& scene) 
  {

  }

  void RelativisticIntegrator::preprocess(const Scene& scene) 
  {

  }

} // namespace Itg
