#pragma once

#ifndef SPOT_LIGHT_HPP
#define SPOT_LIGHT_HPP

#include "Light.hpp"
#include "VisibilityTester.hpp"
#include <Geometry/Interactions/Interaction.hpp>

namespace Luz 
{

    class SpotLight : public Light
    {
        private:

            Point3 from{};
            Vec3 axis{};
            float cutoff{0.f};
            float falloff{0.f};

        public:


            SpotLight(std::shared_ptr<Spectrum> intensity, std::shared_ptr<Spectrum> scale, Point3 from, Point3 to, float cutoff, float fallof)
            : Light(intensity, scale), from(from), axis(::normalize(to - from)), cutoff(cutoff), falloff(fallof)
            {
                this->flag = LightFlag::SPOT;
            };

            SampledSpectrum sampleLi(const Geo::Interaction& hit, 
                                    const Point2&, 
                                    const ssrt::SampledWavelengths& lambdas,
                                    Vec3* wi, float* pdf, VisibilityTester* vis) const override
            {
                
                Vec3 dir = from - hit.p;
                Interaction lpos; lpos.p = from;

                *vis = VisibilityTester(hit, lpos);
                *wi = normalize(dir);
                *pdf = 1.0f;

                float angle = std::acos(dot(-(*wi), axis)) * (180.0 / M_PI);

                double spot = 1.0;

                if(angle >= cutoff)
                {
                    return SampledSpectrum(0.f);
                }
                else if(angle >= falloff)
                {
                    spot = (cutoff - angle) / (cutoff - falloff);
                }           
                
                SampledSpectrum I = intensity->sample(lambdas);
                SampledSpectrum S = scale->sample(lambdas);

                return I * S * spot;
            }
    };

}

#endif //< SPOT_LIGHT_HPP