#pragma once

#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "Shape.hpp"

namespace Geo {
    class Sphere : public Shape 
    {
        private:
        
            float radius;
            float zMin, zMax;
            float thetaMin, thetaMax, phiMax;
            
        public:
        
            Sphere(float radius, float zmin, float zmax, float phimax, bool reverseOrientation, bool tSwapHandedness)
                    : Shape(reverseOrientation, tSwapHandedness),
                      radius(radius), 
                      zMin(clamp(std::min(zmin, zmax), -radius, radius)), 
                      zMax(clamp(std::max(zmin, zmax), -radius, radius)),
                      thetaMin(std::acos(clamp(zMin / radius, -1.0f, 1.0f))),
                      thetaMax(std::acos(clamp(zMax / radius, -1.0f, 1.0f))),
                      phiMax(degToRad(clamp(phimax, 0.0f, 360.0f))){};

            bool intersect(const Ray &r, float *tHit, SurfaceInteraction *sf, bool testAlphaTexture = true) const override;
            bool intersectP(const Ray &r, bool testAlphaTexture = true) const override;
            Bounds3f objectBound() const override;
            float area() const override;
            Interaction sample(const Point2& u, float* pdf) const override;
            Interaction sample(const Interaction& ref, const Point2& u, float* pdf) const override;
            float pdf(const Interaction& ref, const Vec3& wi) const override;
    };
} // namespace rt
#endif
