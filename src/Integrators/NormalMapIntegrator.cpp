#include "NormalMapIntegrator.hpp"
#include "Geometry/Interactions/SurfaceInteraction.hpp"


namespace Itg 
{
    std::optional<Color> NormalMapIntegrator::li(const RayDifferential& ray, const Scene& scene, Sam::Sampler&) const {

        SurfaceInteraction isect;

        if(!scene.intersect(ray, &isect))
            return std::nullopt;
        
        return static_cast<Color>((isect.n + Normal3{1, 1, 1})/2.f);
    }

};