#pragma once

#ifndef AREA_LIGHT_HPP
#define AREA_LIGHT_HPP

#include "Light.hpp"

#include "Geometry/Shapes/Shape.hpp"

#include <memory>

namespace Luz 
{
    class AreaLight : public Light
    {

        public:
        
            std::shared_ptr<Geo::Shape> shape{nullptr};
            std::shared_ptr<const Transform> O2W{nullptr};
            std::shared_ptr<const Transform> W2O{nullptr};

            AreaLight(Color intensity, Color scale)
            : Light(intensity, scale) {flag = LightFlag::AREA;};

            virtual Color L(const Interaction &intr, const Vec3 &w) const = 0;
            virtual std::shared_ptr<AreaLight> clone() const = 0;
            
            virtual Color sampleLi(const Geo::Interaction& ref, 
                                   const Point2& u, 
                                   Vec3* wi, float* pdf, VisibilityTester* vis) const override = 0;
            virtual float pdf(const Geo::Interaction& ref, const Vec3& wi) const override
            {
                return shape->pdf(ref, wi);
            }
    };
}; //< namespace Luz


#endif //< AREA_LIGHT_HPP