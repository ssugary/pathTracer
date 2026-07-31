#include "InterpoledBackground.hpp"
#include "Geometry/Rays/Ray.hpp"

namespace Prim 
{
    Color InterpoledBackground::sample(const Vec3&, float u, float v) const
    {
        const auto bottomH = ::lerp(u, colors[bl], colors[br]);
        const auto topH = ::lerp(u, colors[tl], colors[tr]);
        return ::lerp(v,bottomH, topH);
    }
}