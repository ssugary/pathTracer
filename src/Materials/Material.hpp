#pragma once

#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "Utils/Spectrum/Spectrum.hpp"
#include "Geometry/Interactions/SurfaceInteraction.hpp"
#include "Utils/common.hpp"
#include "BSDF.hpp"

namespace Mat 
{

    class Material 
    {
        protected:
            // std::shared_ptr<Spectrum> mirror;
            Color mirror;
        public:
            Material(Color mirror) : mirror(mirror) {};
            virtual ~Material() = default;
            virtual Color km() const {return mirror;};    

            virtual Color f(const SurfaceInteraction& si, const Vec3& wo, const Vec3& wi/*, const ssrt::SampledWavelengths& lambdas*/) const = 0;
            virtual Color sampleF(const SurfaceInteraction& si, const Vec3& wo, const Point2& u,
                                            /*const ssrt::SampledWavelengths& lambdas,*/
                                            Vec3* wi, float* pdf) const = 0;
            virtual float pdf(const SurfaceInteraction& si, const Vec3& wo, const Vec3& wi) const = 0;
            virtual std::shared_ptr<BSDF> computeBSDF(const SurfaceInteraction& si) const = 0;
    };

}; //< namespace Mat


#endif //< MATERIAL_HPP