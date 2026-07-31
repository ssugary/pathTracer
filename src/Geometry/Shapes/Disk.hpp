#ifndef DISK_HPP
#define DISK_HPP

#include "Shape.hpp"

namespace Geo 
{
    class Disk : public Shape
    {
        private:
            float height;
            float radius;
            float innerRadius;
            float phiMax;

        public:

            Disk(float height, float radius, float innerRadius, float phimax, bool reverseOrientation, bool tSwapHandedness)
            : Shape(reverseOrientation, tSwapHandedness),
              height(height),
              radius(radius), 
              innerRadius(innerRadius),
              phiMax(clamp(phimax, 0.0f, 360.0f)){};

            bool intersect(const Ray &r, float *tHit, SurfaceInteraction *sf, bool testAlphaTexture = true) const override;
            bool intersectP(const Ray &r, bool testAlphaTexture = true) const override;
            Bounds3f objectBound() const override;
            float area() const override;
            Interaction sample(const Point2& u, float* pdf) const override;
    };

} //< namespace Geo


#endif //< DISK_HPP