#pragma once

#ifndef AMBIENT_LIGHT_HPP
#define AMBIENT_LIGHT_HPP

#include <Light/Light.hpp>
namespace Luz 
{
    class AmbientLight : public Light
    {
        public:

            AmbientLight(std::shared_ptr<Spectrum> intensity, std::shared_ptr<Spectrum> scale) 
            : Light(intensity, scale)
            {
                flag = LightFlag::AMBIENT;
            }
            SampledSpectrum sampleLi(const Geo::Interaction&, 
                                    const Point2&, 
                                    const ssrt::SampledWavelengths& lambdas,
                                    Vec3* wi, float* pdf, VisibilityTester*) const override
            {
                *wi = {0, 0, 0};
                *pdf = 1.0f;

                SampledSpectrum I = intensity->sample(lambdas);
                SampledSpectrum S = scale->sample(lambdas);

                return I * S;
            }
    };
};


#endif //< AMBIENT_LIGHT_HPP