#pragma once

#ifndef DIRECTIONAL_LIGHT_HPP
#define DIRECTIONAL_LIGHT_HPP

#include "Light.hpp"
#include "VisibilityTester.hpp"

namespace Luz 
{

    class DirectionalLight : public Light
    {
        private:

            Vec3 dir;
            float worldRadius;

        public:


            DirectionalLight(std::shared_ptr<Spectrum> intensity, std::shared_ptr<Spectrum> scale, Vec3 dir, float worldRadius)
            : Light(intensity, scale), dir(dir), worldRadius(worldRadius)
            {
                flag = LightFlag::DIRECTIONAL;
            };

            SampledSpectrum sampleLi(const Geo::Interaction& hit, 
                                    const Point2&, 
                                    const ssrt::SampledWavelengths& lambdas,
                                    Vec3* wi, float* pdf, VisibilityTester* vis) const override
            {
                *wi = normalize(-dir);
                *pdf = 1.0f;
                
                Interaction lpos;
                lpos.p = hit.p + (*wi) * worldRadius;
                    
                *vis = VisibilityTester(hit, lpos);
                
                SampledSpectrum I = intensity->sample(lambdas);
                SampledSpectrum S = scale->sample(lambdas);

                return I * S;
            }
    };

}

#endif //< DIRECTIONAL_LIGHT_HPP