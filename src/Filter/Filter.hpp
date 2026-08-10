#pragma once

#ifndef FILTER_HPP
#define FILTER_HPP

#include "Utils/common.hpp"

namespace Fil 
{ class Filter 
    {
        public:
            const Vec2 radius;
            const Vec2 invRadius;

            Filter(const Vec2& r) : radius(r), invRadius(Vec2{1/r[0], 1/r[1]}) {};
            
            virtual ~Filter() = default;
            virtual float evaluate(const Point2& p) const = 0;
    };
};


#endif
