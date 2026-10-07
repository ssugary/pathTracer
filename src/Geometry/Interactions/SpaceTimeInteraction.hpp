#pragma once
#ifndef SPACE_TIME_INTERACTION_HPP
#define SPACE_TIME_INTERACTION_HPP

#include "Utils/common.hpp"
#include "Geometry/Primitives/RelativisticPrimitive.hpp"
#include "SurfaceInteraction.hpp"

namespace Geo 
{

    struct SpaceTimeInteraction
    {
        SurfaceInteraction isect3d;
        Point4 x;
        Vec4 p;
        Vec4 velocity{1.f, 0.f, 0.f, 0.f};
        float tStep{0.f};
        
    };

}


#endif //< RELATIVISTIC_INTEGRATOR_HPP