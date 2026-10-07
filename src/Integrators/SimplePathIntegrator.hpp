#pragma once

#ifndef SIMPLE_PATH_INTEGRATOR_HPP
#define SIMPLE_PATH_INTEGRATOR_HPP

#include "SamplerIntegrator.hpp"
#include "Sampler/UniformLightSampler.hpp"

namespace Itg 
{
    class SimplePathIntegrator : public SamplerIntegrator
    {   
        private:
        
            bool sampleLights;
            bool sampleBSDF;
            Sam::UniformLightSampler lightSampler;

        public:

            SimplePathIntegrator(std::shared_ptr<Cam::Camera> cam, std::shared_ptr<Sam::Sampler> sampler, double maxDepth,
                                 bool sampleLights, bool sampleBSDF)
            : SamplerIntegrator(cam, sampler, maxDepth), sampleLights(sampleLights), sampleBSDF(sampleBSDF) {};

            std::optional<Color> li(const RayDifferential& ray, const Scene& scene, Sam::Sampler& sampler) const override;
            void preprocess(const Scene&) override;

    };

}; //< namespace Itg

#endif //< SIMPLE_PATH_INTEGRATOR_HPP