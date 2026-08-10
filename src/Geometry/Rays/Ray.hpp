#pragma once

#ifndef RAY_HPP
#define RAY_HPP

#include "Geometry/Medium/Medium.hpp"
#include "Utils/common.hpp"

namespace Geo
{
    struct Ray 
    {
        
            Point3 o{};
            Vec3   d{};
            mutable float tMax{0.f};
            mutable float tMin{0.f};
            float time{0.f};
            
            const Medium* medium;

            Ray() = default;
            Ray(const Ray&) = default;
            
            Ray(const Point3& o, const Vec3& d, float tMin=EPSILON, float tMax=INF, float time=0.f, const Medium *medium=nullptr) 
            : o(o), d(avoidZeroDirection(d)), tMax(tMax), tMin(tMin), time(time), medium(medium) {};

        
            Point3 operator()(const float t) const
            {
                return o + d * t;
            }
    };
}

#endif