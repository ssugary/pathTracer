#include "RayCastIntegrator.hpp"
#include "Geometry/Interactions/SurfaceInteraction.hpp"


namespace Itg 
{
    std::optional<Color> RayCastIntegrator::li(const Ray& ray, const Scene& scene, Sam::Sampler&) const {

        SurfaceInteraction isect;

        if(!scene.intersect(ray, &isect))
            return std::nullopt;
        
        if (dot(ray.d, isect.n) > 0) 
           isect.n = -isect.n;


        return isect.primitive->getMaterial()->kd();
    }

};