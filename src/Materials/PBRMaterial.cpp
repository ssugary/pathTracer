#include <Geometry/Interactions/SurfaceInteraction.hpp>
#include "PBRMaterial.hpp"
#include <Geometry/Textures/Texture.hpp>

namespace Mat 
{

    PBRMaterial::PBRMaterial(std::shared_ptr<Geo::Texture<Color>> kd,
                              std::shared_ptr<Geo::Texture<Color>> eta,
                              std::shared_ptr<Geo::Texture<Color>> k,
                              std::shared_ptr<Geo::Texture<float>> roughness,
                              std::shared_ptr<Geo::Texture<float>> ior,
                              MatType type,
                              std::shared_ptr<Geo::Texture<Color>> normalMap,
                              Color mirror)
    : Material(mirror), kd(kd), roughness(roughness), ior(ior), eta(eta), k(k),
      normalMap(normalMap), type(type) {}

    PBRMaterial::PBRMaterial(Color kd, Color eta, Color k, float roughness, float ior,
                              MatType type, std::shared_ptr<Geo::Texture<Color>> normalMap, Color mirror)
    : Material(mirror), type(type)
    {
        this->kd        = std::make_shared<Geo::ConstantTexture<Color>>(kd);
        this->eta       = std::make_shared<Geo::ConstantTexture<Color>>(eta);
        this->k         = std::make_shared<Geo::ConstantTexture<Color>>(k);
        this->roughness = std::make_shared<Geo::ConstantTexture<float>>(roughness);
        this->ior       = std::make_shared<Geo::ConstantTexture<float>>(ior);
        this->normalMap = normalMap;
    }

    std::shared_ptr<BSDF> PBRMaterial::computeBSDF(const SurfaceInteraction& si) const 
    {
        Normal3 ns = applyNormalMap(si, normalMap);
        auto bsdf = std::make_shared<BSDF>(ns);

        Color kdVal   = kd->evaluate(si);
        float roughVal = roughness->evaluate(si);
        float iorVal   = ior->evaluate(si);

        if (type == MatType::MATTE) 
            bsdf->add(std::make_shared<LambertianReflection>(kdVal));
        else if (type == MatType::DIELECTRIC) 
        {
            
            auto fresnel = std::make_shared<FresnelDielectric>(1.f, iorVal);
            auto dist = std::make_shared<TrowbridgeReitzDistribution>(roughVal, roughVal, true);

            bsdf->add(std::make_shared<MicrofacetReflection>(Color(1.f), dist, 1.f, iorVal, fresnel));
            bsdf->add(std::make_shared<MicrofacetTransmission>(Color(1.f), dist, 1.f, iorVal, fresnel));
        } 
        else if (type == MatType::CONDUCTOR) 
        {
            Color etaVal = eta->evaluate(si);
            Color kVal   = k->evaluate(si);

            auto fresnel = std::make_shared<FresnelConductor>(Color(1.f), etaVal, kVal);
            auto dist = std::make_shared<TrowbridgeReitzDistribution>(roughVal, roughVal, true);
            bsdf->add(std::make_shared<MicrofacetReflection>(Color(1.f), dist, 1.f, iorVal, fresnel));
        }



        return bsdf;
    }

    Color PBRMaterial::f(const SurfaceInteraction& si, const Vec3& wo, const Vec3& wi/*, const ssrt::SampledWavelengths& lambdas*/) const 
    {
        return computeBSDF(si)->f(wo, wi);
    }
    Color PBRMaterial::sampleF(const SurfaceInteraction& si, const Vec3& wo, const Point2& u,
                                    /*const ssrt::SampledWavelengths& lambdas,*/
                                    Vec3* wi, float* pdf) const 
    {
        return computeBSDF(si)->sampleF(wo, u, wi, pdf);
    }
    float PBRMaterial::pdf(const SurfaceInteraction& si, const Vec3& wo, const Vec3& wi) const 
    {
        return computeBSDF(si)->pdf(wo, wi);
    }
};