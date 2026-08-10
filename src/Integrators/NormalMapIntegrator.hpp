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

            std::optional<SampledSpectrum> li(const Ray& ray, const Scene& scene, Sam::Sampler& sampler, ssrt::SampledWavelengths) const override;
            

    };

}; //< namespace Itg


#endif //< NORMAL_MAP_INTEGRATOR_HPP