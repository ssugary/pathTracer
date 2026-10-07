#pragma once
#ifndef SPACE_TIME_INTERACTION_HPP
#define SPACE_TIME_INTERACTION_HPP

#include "Utils/common.hpp"
#include "Geometry/Primitives/RelativisticPrimitive.hpp"
namespace Geo 
{

    class SpaceTimeInteraction
    {
        public:

            Point4 x;
            Vec4 photon;
            Vec4 medium;
            Point2 uv;

            const Prim::RelativisticPrimitive* prim;

        
    };

}


#endif //< RELATIVISTIC_INTEGRATOR_HPP