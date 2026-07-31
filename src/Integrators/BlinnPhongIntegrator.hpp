#ifndef BLINN_PHONG_INTEGRATOR_HPP
#define BLINN_PHONG_INTEGRATOR_HPP

#include "SamplerIntegrator.hpp"
#include <memory>

namespace Itg 
{
    class BlinnPhongIntegrator : public SamplerIntegrator
    {
        public:
        
            BlinnPhongIntegrator(const std::shared_ptr<Cam::Camera>& cam, std::shared_ptr<Sam::Sampler> sampler, int maxDepth)
            : SamplerIntegrator(cam, sampler, maxDepth)
            {};

            std::optional<Color> li(const Ray& ray, const Scene& scene, Sam::Sampler& sampler) const override;
    };
}; //< namespace Itg


#endif //< BLINN_PHONG_INTEGRATOR_HPP