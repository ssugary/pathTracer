#pragma once

#ifndef FLAT_MATERIAL_HPP
#define FLAT_MATERIAL_HPP

#include "Material.hpp"

namespace Mat 
{

    class FlatMaterial : public Material 
    {
        private:
            std::shared_ptr<Spectrum> color;
        public:
            FlatMaterial(std::shared_ptr<Spectrum> color, std::shared_ptr<Spectrum> mirror) 
            : Material(mirror), color(color) {};
            std::shared_ptr<Spectrum> kd() const override {return color;};
            std::shared_ptr<Spectrum> km() const override {return mirror;};    
            SampledSpectrum f(const Vec3&, const Vec3&, const Normal3&, const ssrt::SampledWavelengths& lambdas) const override 
            {
                return color ? color->sample(lambdas) : ssrt::SampledSpectrum(0.f);
            }
            SampledSpectrum sampleF(const Vec3&, const Normal3&, const Point2&,
                                    const ssrt::SampledWavelengths& lambdas,
                                    Vec3*, float*) const override 
            {
                return color ? color->sample(lambdas) : ssrt::SampledSpectrum(0.f);
            }
    };

}; //< namespace Mat


#endif //< FLAT_MATERIAL_HPP