#include "RayCastIntegrator.hpp"
#include "Geometry/Interactions/SurfaceInteraction.hpp"


namespace Itg 
{
    std::optional<SampledSpectrum> RayCastIntegrator::li(const Ray& ray, const Scene& scene, Sam::Sampler&, ssrt::SampledWavelengths lambdas) const {

        SurfaceInteraction isect;

        if(!scene.intersect(ray, &isect))
            return std::nullopt;
        
        if (dot(ray.d, isect.n) > 0) 
           isect.n = -isect.n;

        auto kd = isect.primitive->getMaterial()->kd();
        if (kd)
            return kd->sample(lambdas);
            
        return ssrt::SampledSpectrum(0.f);
    }

};