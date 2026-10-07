#pragma once
#ifndef SOLVER_HPP
#define SOLVER_HPP

#include <memory>

#include <Geometry/Metric/Metric.hpp>
#include <Geometry/Rays/GeodesicRay.hpp>

namespace Geo 
{
    struct StepResult 
    {
      GeodesicRay ray;
      float recommendedNextStep; 
    };

    class GeodesicSolver 
    {
        public:

          virtual ~GeodesicSolver() = default;
          virtual StepResult step(const GeodesicRay& ray, const Metric& metric, float dl) const = 0;

    };

} // namespace Geo


#endif // SOLVER_HPP      
