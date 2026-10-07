#pragma once
#ifndef RELATIVISTIC_PRIMITIVE_HPP
#define RELATIVISTIC_PRIMITIVE_HPP

#include "Geometry/Rays/GeodesicRay.hpp"

namespace Geo 
{
    struct SpaceTimeInteraction;
}

namespace Prim 
{

    struct GeodesicStep 
    { 
        GeodesicRay prevRay;
        GeodesicRay currRay;
        float dl;

        GeodesicRay lerp(float t) const;
    };

    class RelativisticPrimitive 
    {

        public: 

            virtual ~RelativisticPrimitive() = default;
            virtual bool intersect(const GeodesicStep& step,  Geo::SpaceTimeInteraction* isect) const = 0;
            virtual bool intersectP(const GeodesicStep& step) const = 0;
    };

};


#endif