#pragma once

#ifndef RAY_DIFFERENTIAL_HPP
#define RAY_DIFFERENTIAL_HPP

#include "Ray.hpp"

namespace Geo{
    class RayDifferential : public Ray
    {
        public:

        Point3 rxOrigin;
        Point3 ryOrigin;
        Vec3 rxDirection;
        Vec3 ryDirection;
        bool hasDifferentials{false};
        
        RayDifferential() 
        { 
            hasDifferentials = false; 
        }

        RayDifferential(const Point3 &o, const Vec3 &d, float tMin=0.f, float tMax=INF, float time=0.f, const Medium *medium = nullptr)
        : Ray(o, d,tMin, tMax, time, medium) 
        {
            hasDifferentials = false; 
        }
        RayDifferential(const Ray &ray) : Ray(ray) 
        {
            hasDifferentials = false; 
        }

        void scaleDifferentials(const float s) 
        {
            rxOrigin = o + (rxOrigin - o) * s;
            ryOrigin = o + (ryOrigin - o) * s;
            rxDirection = d + (rxDirection - d) * s;
            ryDirection = d + (ryDirection - d) * s;
        }
    };
}
#endif