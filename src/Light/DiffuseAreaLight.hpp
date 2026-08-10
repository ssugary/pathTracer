#pragma once

#ifndef DIFFUSE_AREA_LIGHT_HPP
#define DIFFUSE_AREA_LIGHT_HPP

#include "AreaLight.hpp"
#include "VisibilityTester.hpp"

namespace Luz 
{
    class DiffuseAreaLight : public AreaLight
    {
        private:

            bool twoSided;
            float alpha; //< TODO FloatTexture

            inline bool alphaMask(const Geo::Interaction&) const
            {
                if(!alpha)
                        return false;
                return false;
            }
        public:

            DiffuseAreaLight(std::shared_ptr<Spectrum> intensity, std::shared_ptr<Spectrum> scale, bool twoSided = false, float alpha = 0.f)
            : AreaLight(intensity, scale), twoSided(twoSided), alpha(alpha)
            {
                shape = nullptr;
                flag = LightFlag::AREA;
            }

            SampledSpectrum L(const Geo::Interaction& intr, const Vec3& w, const ssrt::SampledWavelengths& lambdas) const override
            {
                if(!twoSided && dot(intr.n, w) <= SHADOW_EPSILON)
                    return SampledSpectrum(0.f);
                if(alphaMask(intr))
                    return SampledSpectrum(0.f);

                SampledSpectrum I = intensity->sample(lambdas);
                SampledSpectrum S = scale->sample(lambdas);

                return I * S;
            }

            std::shared_ptr<AreaLight> clone() const override
            {
                std::shared_ptr<AreaLight> cloned = std::make_shared<DiffuseAreaLight>(intensity, scale, twoSided);
                
                cloned->shape = this->shape;
                cloned->O2W = this->O2W;
                cloned->W2O = this->W2O;

                return cloned;
            }

            SampledSpectrum sampleLi(const Geo::Interaction& ref, 
                                    const Point2& u, 
                                    const ssrt::SampledWavelengths& lambdas,
                                    Vec3* wi, float* pdf, VisibilityTester* vis) const override
            {
                
                if (!shape) 
                {
                    *pdf = 0.f;
                    return SampledSpectrum(0.f);
                }  
                
                Geo::Interaction pShape = shape->sample(u, pdf);

                if(*pdf <= 0.f)
                {
                    *pdf = 0.f;
                    return SampledSpectrum(0.f);
                }
                
            
                if (O2W) 
                {
                    pShape.p =  (*O2W)(pShape.p, pShape.pError, &pShape.pError);
                    pShape.n =  normalize((*O2W)(pShape.n));
                }

                Vec3 d = pShape.p - ref.p;
                auto dist2 = sqrLength(d);

                if (dist2 <= SHADOW_EPSILON) 
                {
                    *pdf = 0.f;
                    return SampledSpectrum(0.f);
                }
                

                *wi = normalize(d);
                float cosThetaLight = std::abs(dot(pShape.n, -*wi));
                
                if (cosThetaLight < EPSILON_6) 
                { 
                    *pdf = 0.f; return SampledSpectrum(0.f); 
                }

                *pdf *= dist2 / cosThetaLight;
                *vis = VisibilityTester(ref, pShape);

                return L(pShape, -*wi, lambdas);
            }
    };
};

#endif //< DIFFUSE_AREA_LIGHT_HPP