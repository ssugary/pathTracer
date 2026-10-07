#pragma once

#ifndef SAMPLER_INTEGRATOR_HPP
#define SAMPLER_INTEGRATOR_HPP

#include "Cameras/Camera.hpp"
#include "Integrator.hpp"
#include "Sampler/Sampler.hpp"
#include "Utils/Spectrum/Spectrum.hpp"
#include <memory>
#include <optional>

namespace Itg 
{

    class SamplerIntegrator : public Integrator
    {
        protected:
        
            std::shared_ptr<Cam::Camera> camera;
            std::shared_ptr<Sam::Sampler> sampler;
            double maxDepth;
        
        public:

            SamplerIntegrator(std::shared_ptr<Cam::Camera> cam, std::shared_ptr<Sam::Sampler> sampler, double maxDepth)
            : camera(cam), sampler(sampler), maxDepth(maxDepth)
            {}

            virtual void render(const Scene& scene) override;
            virtual std::optional<Color> li(const RayDifferential& ray, const Scene& scene, Sam::Sampler& sampler) const = 0;
            virtual void preprocess(const Scene&){};
    };

} //< namespace Itg


#endif //< SAMPLER_INTEGRATOR_HPP