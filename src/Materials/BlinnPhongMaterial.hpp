#pragma once

#ifndef BLINN_PHONG_MATERIAL_HPP
#define BLINN_PHONG_MATERIAL_HPP

#include "Material.hpp"

namespace Mat{

    class BlinnPhongMaterial : public Material{
        private:
            std::shared_ptr<Spectrum> diffuse;   //< color that indicates how much diffuse color is reflected.
            std::shared_ptr<Spectrum> specular;  //< color that represents the color of the specular highlights.
            std::shared_ptr<Spectrum> ambient;   //< color that represents how much the incoming light is reflected.
            float glossiness;  //< value that control how narrowed is the specular highlight in the scene.
        public:
            BlinnPhongMaterial(std::shared_ptr<Spectrum> mirror = nullptr) : Material(mirror), diffuse(), specular(), ambient(), glossiness() {};

            BlinnPhongMaterial(std::shared_ptr<Spectrum> diffuse, 
                               std::shared_ptr<Spectrum> specular, 
                               std::shared_ptr<Spectrum> ambient, float glossiness, 
                               std::shared_ptr<Spectrum> mirror = nullptr) 
            : Material(mirror), diffuse(diffuse), specular(specular), ambient(ambient), glossiness(glossiness) {};

            std::shared_ptr<Spectrum>          km() const override {return mirror;};   //< mirror's getter
            virtual std::shared_ptr<Spectrum>  kd() const override {return diffuse;};  //< diffuse coeficient's getter
            virtual std::shared_ptr<Spectrum>  ks() const {return specular;};          //< specular coeficient's getter
            virtual std::shared_ptr<Spectrum>  ka() const {return ambient;};           //< ambient coeficient's getter

            virtual float gg() const {return glossiness;};        //< glossiness's getter 
            

            SampledSpectrum f(const Vec3& wo, const Vec3& wi, const Normal3& n, const ssrt::SampledWavelengths& lambdas) const override;

            SampledSpectrum sampleF(const Vec3& wo, const Normal3& n, const Point2& u,
                                            const ssrt::SampledWavelengths& lambdas,
                                            Vec3* wi, float* pdf) const override;

    };

}
#endif