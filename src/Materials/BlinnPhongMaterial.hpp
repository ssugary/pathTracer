#pragma once

#ifndef BLINN_PHONG_MATERIAL_HPP
#define BLINN_PHONG_MATERIAL_HPP

#include "Material.hpp"
#include <mutex>

namespace Mat{

    class BlinnPhongMaterial : public Material{
        private:
            Color diffuse;   //< color that indicates how much diffuse color is reflected.
            Color specular;  //< color that represents the color of the specular highlights.
            Color ambient;   //< color that represents how much the incoming light is reflected.
            float glossiness;  //< value that control how narrowed is the specular highlight in the scene.
        public:
            BlinnPhongMaterial(Color mirror = Color()) : Material(mirror), diffuse(), specular(), ambient(), glossiness() {};

            BlinnPhongMaterial(Color diffuse, 
                               Color specular, 
                               Color ambient, float glossiness, 
                               Color mirror = Color()) 
            : Material(mirror), diffuse(diffuse), specular(specular), ambient(ambient), glossiness(glossiness) {};

            virtual Color  kd() const {return diffuse;};  //< diffuse coeficient's getter
            virtual Color  ks() const {return specular;};          //< specular coeficient's getter
            virtual Color  ka() const {return ambient;};           //< ambient coeficient's getter

            virtual float gg() const {return glossiness;};        //< glossiness's getter 
            

            Color f(const SurfaceInteraction& si, const Vec3& wo, const Vec3& wi/*, const ssrt::SampledWavelengths& lambdas*/) const override;

            Color sampleF(const SurfaceInteraction& si, const Vec3& wo, const Point2& u,
                                            /*const ssrt::SampledWavelengths& lambdas,*/
                                            Vec3* wi, float* pdf) const override;
            float pdf(const SurfaceInteraction& si, const Vec3& wo, const Vec3& wi) const override;
            std::shared_ptr<BSDF> computeBSDF(const SurfaceInteraction& si) const override;

    };

}
#endif