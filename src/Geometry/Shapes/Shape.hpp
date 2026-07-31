#ifndef SHAPE_HPP 
#define SHAPE_HPP

#include "Geometry/Interactions/SurfaceInteraction.hpp"
#include "Geometry/Transformation/Transform.hpp"

namespace Geo 
{
    class Shape 
    {            
        public:
    

            const bool reverseOrientation;
            const bool tSwapHandedness;

            Shape(bool reverseOrientation, bool tSwapHandedness)
            : reverseOrientation(reverseOrientation), tSwapHandedness(tSwapHandedness)
            {};

            
            virtual bool intersectP(const Ray &r, bool testAlphaTexture = true) const;
            virtual bool intersect(const Ray &r, float *tHit, SurfaceInteraction *sf, bool testAlphaTexture = true) const = 0;
            virtual Bounds3f objectBound() const = 0;
            virtual float area() const = 0;
            virtual Interaction sample(const Point2& u, float* pdf) const = 0;
            virtual Interaction sample(const Interaction& ref, const Point2& u, float* pdf) const;
        };
}

#endif