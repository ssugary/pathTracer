#pragma once

#ifndef NORMAL_MAP_INTEGRATOR_HPP
#define NORMAL_MAP_INTEGRATOR_HPP

#include "Integrators/SamplerIntegrator.hpp"
namespace Itg 
{

    class NormalMapIntegrator : public SamplerIntegrator
    {
        public:
            NormalMapIntegrator(const std::shared_ptr<Cam::Camera>& cam, std::shared_ptr<Sam::Sampler> sampler, int maxDepth)
            : SamplerIntegrator(cam, sampler, maxDepth)
            {};

            std::optional<Color> li(const RayDifferential& ray, const Scene& scene, Sam::Sampler& sampler) const override;
            

    };

}; //< namespace Itg


#endif //< NORMAL_MAP_INTEGRATOR_HPP