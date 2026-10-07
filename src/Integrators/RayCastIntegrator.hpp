#pragma once

#ifndef RAY_CAST_INTEGRATOR_HPP
#define RAY_CAST_INTEGRATOR_HPP

#include "Integrators/SamplerIntegrator.hpp"
namespace Itg 
{

    class RayCastIntegrator : public SamplerIntegrator
    {
        public:
            RayCastIntegrator(const std::shared_ptr<Cam::Camera>& cam, std::shared_ptr<Sam::Sampler> sampler, int maxDepth)
            : SamplerIntegrator(cam, sampler, maxDepth)
            {};

            std::optional<Color> li(const RayDifferential& ray, const Scene& scene, Sam::Sampler& sampler) const override;
            

    };

}; //< namespace Itg


#endif //< NORMAL_MAP_INTEGRATOR_HPP