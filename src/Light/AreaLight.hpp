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
        
            std::shared_ptr<Geo::Shape> shape;
            std::shared_ptr<const Transform> O2W{nullptr};
            std::shared_ptr<const Transform> W2O{nullptr};

            AreaLight(std::shared_ptr<Spectrum> intensity, std::shared_ptr<Spectrum> scale)
            : Light(intensity, scale) {flag = LightFlag::AREA;};

            virtual SampledSpectrum L(const Interaction &intr, const Vec3 &w, const ssrt::SampledWavelengths& lambdas) const = 0;
            virtual std::shared_ptr<AreaLight> clone() const = 0;
            
            virtual SampledSpectrum sampleLi(const Geo::Interaction& ref, 
                                   const Point2& u, 
                                   const ssrt::SampledWavelengths& lambdas,
                                   Vec3* wi, float* pdf, VisibilityTester* vis) const override = 0;
    };
}; //< namespace Luz


#endif //< AREA_LIGHT_HPP