#include "RayCastIntegrator.hpp"
#include "Materials/FlatMaterial.hpp"
#include "Geometry/Interactions/SurfaceInteraction.hpp"


namespace Itg 
{
    std::optional<Color> RayCastIntegrator::li(const RayDifferential& ray, const Scene& scene, Sam::Sampler&) const 
    {
        SurfaceInteraction isect;

        if(!scene.intersect(ray, &isect))
            return std::nullopt;
        
        if (dot(ray.d, isect.n) > 0) 
           isect.n = -isect.n;

        auto fm = std::dynamic_pointer_cast<Mat::FlatMaterial>(isect.primitive->getMaterial());

        return fm->kd();
     }

};
