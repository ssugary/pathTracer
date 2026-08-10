#include "NormalMapIntegrator.hpp"
#include "Geometry/Interactions/SurfaceInteraction.hpp"
#include "Utils/Spectrum/OtherSpectrum.hpp"

namespace Itg 
{
    std::optional<SampledSpectrum> NormalMapIntegrator::li(const Ray& ray, const Scene& scene, Sam::Sampler&, ssrt::SampledWavelengths lambdas) const {

        SurfaceInteraction isect;

        if(!scene.intersect(ray, &isect))
            return std::nullopt;
        
        
        Color rgb = static_cast<Color>((isect.n + Normal3{1, 1, 1}) * 0.5f);

        ssrt::RGBAlbedoSpectrum spec(rgb.r, rgb.g, rgb.b);

        return spec.sample(lambdas);
    }

};