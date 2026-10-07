#pragma once

#include <Geometry/Textures/Texture.hpp>
#ifndef FLAT_MATERIAL_HPP
#define FLAT_MATERIAL_HPP

#include "Material.hpp"

namespace Mat 
{

    class FlatMaterial : public Material 
    {
        private:
            Color color;
        public:
            FlatMaterial(Color color, Color mirror) 
            : Material(mirror), color(color) {};
            Color kd() const {return color;};
            Color f(const SurfaceInteraction& si, const Vec3& wo, const Vec3& wi/*, const ssrt::SampledWavelengths& lambdas*/) const override 
            {
                return computeBSDF(si)->f(wo, wi);
            }
            Color sampleF(const SurfaceInteraction& si, const Vec3& wo, const Point2& u,
                                    /*const ssrt::SampledWavelengths& lambdas,*/
                                    Vec3* wi, float* pdf) const override 
            {
                return computeBSDF(si)->sampleF(wo, u, wi, pdf);
            }
            float pdf(const SurfaceInteraction& si, const Vec3& wo, const Vec3& wi) const override 
            {
                return computeBSDF(si)->pdf(wo, wi);
            };

            std::shared_ptr<BSDF> computeBSDF(const SurfaceInteraction& si) const override 
            {

                Normal3 ns = Geo::applyNormalMap(si, nullptr);
                auto bsdf = std::make_shared<BSDF>(ns);
                bsdf->add(std::make_shared<FlatBxDF>(color));
                return bsdf;
            }
    };

}; //< namespace Mat


#endif //< FLAT_MATERIAL_HPP