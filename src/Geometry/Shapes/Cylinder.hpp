#ifndef CYLINDER_HPP
#define CYLINDER_HPP

#include "Shape.hpp"

namespace Geo 
{

    class Cylinder : public Shape
    {
        private:

            const float radius;
            const float zMin, zMax;
            const float phiMax;

        public:
            Cylinder(float radius, float zmin, float zmax, float phimax, bool reverseOrientation, bool tSwapHandedness)
            : Shape(reverseOrientation, tSwapHandedness),
                      radius(radius), 
                      zMin(std::min(zmin, zmax)), 
                      zMax(std::max(zmin, zmax)),
                      phiMax(clamp(phimax, 0.0f, 360.0f)){};

            bool intersect(const Ray &r, float *tHit, SurfaceInteraction *sf, bool testAlphaTexture = true) const override;
            bool intersectP(const Ray &r, bool testAlphaTexture = true) const override;
            Bounds3f objectBound() const override;
            float area() const override;
            Interaction sample(const Point2& u, float* pdf) const override;

    };

};

#endif //< CYLINDER_HPP