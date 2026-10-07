#pragma once 

#ifndef PBR_MATERIAL_HPP
#define PBR_MATERIAL_HPP

#include "Material.hpp"
#include "Geometry/Textures/Texture.hpp"
#include "Microfacets.hpp"

namespace Mat 
{
    enum class MatType 
    {
        DIELECTRIC,
        CONDUCTOR,
        MATTE
    };

    class PBRMaterial : public Material 
    {
        private:

            std::shared_ptr<Geo::Texture<Color>> kd;
            std::shared_ptr<Geo::Texture<float>> roughness;
            std::shared_ptr<Geo::Texture<float>> ior;
            std::shared_ptr<Geo::Texture<Color>> eta;
            std::shared_ptr<Geo::Texture<Color>> k;
            std::shared_ptr<Geo::Texture<Color>> normalMap;
            MatType type;

        public:

            PBRMaterial(std::shared_ptr<Geo::Texture<Color>> kd,
                        std::shared_ptr<Geo::Texture<Color>> eta,
                        std::shared_ptr<Geo::Texture<Color>> k,
                        std::shared_ptr<Geo::Texture<float>> roughness,
                        std::shared_ptr<Geo::Texture<float>> ior,
                        MatType type = MatType::MATTE,
                        std::shared_ptr<Geo::Texture<Color>> normalMap = nullptr,
                        Color mirror = Color());

            PBRMaterial(Color kd, Color eta, Color k, float roughness, float ior, MatType type = MatType::MATTE, std::shared_ptr<Geo::Texture<Color>> normalMap = nullptr, Color mirror = Color());

            std::shared_ptr<BSDF> computeBSDF(const SurfaceInteraction& si) const override;

            Color f(const SurfaceInteraction& si, const Vec3& wo, const Vec3& wi/*, const ssrt::SampledWavelengths& lambdas*/) const override;
            Color sampleF(const SurfaceInteraction& si, const Vec3& wo, const Point2& u,
                                            /*const ssrt::SampledWavelengths& lambdas,*/
                                            Vec3* wi, float* pdf) const override;
            float pdf(const SurfaceInteraction& si, const Vec3& wo, const Vec3& wi) const override;
    };
}

#endif //< PBR_MATERIAL_HPP