#pragma once

#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "Geometry/Interactions/Interaction.hpp"
#include "Utils/Spectrum/Spectrum.hpp"

#include <memory>

namespace Luz 
{
    class VisibilityTester;

    enum class LightFlag 
    {
        POINT=0,
        DIRECTIONAL,
        AMBIENT, 
        SPOT,
        AREA,
    };
    class Light 
    {
        protected:

            std::shared_ptr<Spectrum> intensity;
            std::shared_ptr<Spectrum> scale;
            // Geo::Transform lightToWorld;
            // Geo::Transform worldToLight;
            
        public:
            
            LightFlag flag;

            Light(std::shared_ptr<Spectrum> intensity, std::shared_ptr<Spectrum> scale)
            : intensity(intensity), scale(scale) {};

            virtual ~Light() = default;
            virtual SampledSpectrum sampleLi(const Geo::Interaction& ref, 
                                    const Point2& u, 
                                    const ssrt::SampledWavelengths& lambdas,
                                    Vec3* wi, float* pdf, VisibilityTester* vis) const = 0;

    };
}; //< namespace Luz


#endif //< LIGHT_HPP