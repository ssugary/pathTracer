#pragma once
#ifndef _INTEGRATOR_HPP
#define _INTEGRATOR_HPP

#include "Sampler/Sampler.hpp"
#include "Integrators/Integrator.hpp"
#include "Geometry/Rays/GeodesicRay.hpp"
#include "Cameras/RelativisticCamera.hpp"
#include "Geometry/Metric/Metric.hpp"
#include "Geometry/Solver/RK4Solver.hpp"

namespace Itg 
{

    class RelativisticIntegrator : public Integrator
    {
        private:
          
          std::shared_ptr<Cam::RelativisticCamera> cam;
          std::shared_ptr<Sam::Sampler> sampler;
          std::shared_ptr<Geo::Metric> metric;

          int maxSteps{2000};
          const float rEscape{30.f};
          const float dLambda{0.05f};

          const RK4Solver solver;

          float computeRedshift(const GeodesicRay& ray, float rDisk) const;

        public:
            RelativisticIntegrator(std::shared_ptr<Cam::RelativisticCamera> cam, std::shared_ptr<Sam::Sampler> sampler, std::shared_ptr<Metric> metric, int maxSteps, float rEscape, float dLambda);

            void render(const Scene& scene) override;

            virtual std::optional<Color> li(const GeodesicRay& ray, const Scene& scene, Sam::Sampler& sampler) const;
            virtual void preprocess(const Scene&);

    };

} // namespace Itg

#endif // RELATIVISTIC_INTEGRATOR_HPP
