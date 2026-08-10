#pragma once

#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "Utils/Spectrum/Spectrum.hpp"
#include "Utils/common.hpp"
#include <memory>

namespace Mat 
{

    class Material 
    {
        protected:
            std::shared_ptr<Spectrum> mirror;
        public:
            Material(std::shared_ptr<Spectrum> mirror) : mirror(mirror) {};
            virtual ~Material() = default;
            virtual std::shared_ptr<Spectrum> kd() const = 0;
            virtual std::shared_ptr<Spectrum> km() const = 0;    

            virtual SampledSpectrum f(const Vec3& wo, const Vec3& wi, const Normal3& n, const ssrt::SampledWavelengths& lambdas) const = 0;
            virtual SampledSpectrum sampleF(const Vec3& wo, const Normal3& n, const Point2& u,
                                            const ssrt::SampledWavelengths& lambdas,
                                            Vec3* wi, float* pdf) const = 0;
    };

}; //< namespace Mat


#endif //< MATERIAL_HPP