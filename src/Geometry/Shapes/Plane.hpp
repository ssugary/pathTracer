#ifndef PLANE_HPP
#define PLANE_HPP

#include "Shape.hpp"
#include <memory>

namespace Geo{
    class Plane : public Shape {
        private:
            Point3 p;
            Normal3 n;
        public:
        Plane(const Point3& p, const Normal3& n, bool reverseOrientation, bool tSwapHandedness)
        : Shape(reverseOrientation, tSwapHandedness), p(p), n(n) {};

        bool intersect(const Ray &r, float *tHit, SurfaceInteraction *sf, bool testAlphaTexture = true) const override;
        bool intersectP(const Ray &r, bool testAlphaTexture = true) const override;
        Bounds3f objectBound() const override;
        float area() const override;
    };
}

#endif
