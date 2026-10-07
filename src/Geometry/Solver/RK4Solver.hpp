#pragma once
#ifndef RK4SOLVER_HPP
#define RK4SOLVER_HPP

#include <Geometry/Solver/Solver.hpp>

namespace Geo 
{
    class RK4Solver : public GeodesicSolver
    {
        public:

            RK4Solver();
            
            StepResult step(const GeodesicRay& ray, const Metric& metric, float dl) const override;

    };

} // namespace Geo


#endif // RK4SOLVER_HPP     
