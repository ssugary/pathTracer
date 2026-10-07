#include "RelativisticPrimitive.hpp"


namespace Prim 
{
    GeodesicRay GeodesicStep::lerp(float t) const
    {
        return GeodesicRay(::lerp(t, prevRay.x, currRay.x), ::lerp(t, prevRay.p, currRay.p));
    }
};