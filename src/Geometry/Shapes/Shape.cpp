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
}