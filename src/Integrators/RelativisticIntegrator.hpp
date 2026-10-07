#pragma once
#ifndef RELATIVISTIC_INTEGRATOR_HPP
#define RELATIVISTIC_INTEGRATOR_HPP

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

            enum class HitType 
            {
                CAPTURED,
                ESCAPED,
                DISK,
                MAX_STEPS
            };

            struct HitResult 
            {
                HitType type{HitType::CAPTURED};
                Color L{0.f};
                Vec3 escapeDir{0.f, 1.f, 0.f};
            };
            
            
            std::shared_ptr<Cam::RelativisticCamera> cam;
            std::shared_ptr<Sam::Sampler> sampler;
            std::shared_ptr<Geo::Metric> metric;
            std::unique_ptr<Geo::GeodesicSolver> solver;
            int maxSteps{5000};
            float rEscape{1000.f};
            float dLambda{0.05f};
            float rDiskOut{25.f};

            float computeIscoRadius() const;
            float computeRedshift(const GeodesicRay& ray, float rDisk) const;
            Vec3 computeEscapeDirection(const Point4& x, const PhaseSpaceDerivatives& derivs) const;

        public:

            RelativisticIntegrator(std::shared_ptr<Cam::RelativisticCamera> cam,
                                   std::shared_ptr<Sam::Sampler> sampler, 
                                   std::shared_ptr<Metric> metric, 
                                   std::unique_ptr<Geo::GeodesicSolver> solver,
                                   int maxSteps=5000, float rEscape=1000.f, float dLambda=0.05f, float rDiskOut=25.f);

            void render(const Scene& scene) override;

            virtual HitResult li(const GeodesicRay& ray, const Scene& scene, Sam::Sampler& sampler) const;
            virtual void preprocess(const Scene&);

    };

} // namespace Itg

#endif // RELATIVISTIC_INTEGRATOR_HPP
