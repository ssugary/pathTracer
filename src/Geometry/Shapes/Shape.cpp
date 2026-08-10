#include "Shape.hpp"
#include "Geometry/Interactions/SurfaceInteraction.hpp"
#include "Geometry/Rays/Ray.hpp"

namespace Geo 
{
    bool Shape::intersectP(const Ray& r, bool testAlphaTexture) const 
    {
        float tHit = r.tMax;
        SurfaceInteraction sf;
        return intersect(r, &tHit, &sf, testAlphaTexture);
    }
    Interaction Shape::sample(const Interaction& ref, const Point2& u, float* pdf) const
    {
        Interaction it = sample(u, pdf); 

        Vec3 d = it.p - ref.p;
        float dist2 = sqrLength(d);

        if (dist2 == 0.f || *pdf == 0.f)
        {
            *pdf = 0.f;
            return it;
        }

        Vec3 wi = normalize(d);
        float cosThetaLight = std::abs(dot(it.n, -wi));

        if (cosThetaLight < EPSILON_6)
        {
            *pdf = 0.f;
            return it;
        }

        *pdf *= dist2 / cosThetaLight; 

        return it;
    }
}