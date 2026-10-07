#include "BlinnPhongMaterial.hpp"
#include <Geometry/Textures/Texture.hpp>


namespace Mat 
{

    Color BlinnPhongMaterial::f(const SurfaceInteraction& si, const Vec3& wo, const Vec3& wi) const
    {
        return computeBSDF(si)->f(wo, wi);
    }
    Color BlinnPhongMaterial::sampleF(const SurfaceInteraction& si, const Vec3& wo, const Point2& u,
                                    Vec3* wi, float* pdf) const 
    {
        return computeBSDF(si)->sampleF(wo, u, wi, pdf);
    }
    float BlinnPhongMaterial::pdf(const SurfaceInteraction& si, const Vec3& wo, const Vec3& wi) const 
    {
        return computeBSDF(si)->pdf(wo, wi);
    }

    std::shared_ptr<BSDF> BlinnPhongMaterial::computeBSDF(const SurfaceInteraction& si) const
    {
    
        Normal3 ns = Geo::applyNormalMap(si, nullptr);

        auto bsdf = std::make_shared<BSDF>(ns);
        
        bool hasDiffuse  = diffuse.r  > 0.f || diffuse.g  > 0.f || diffuse.b  > 0.f;
        bool hasSpecular = specular.r > 0.f || specular.g > 0.f || specular.b > 0.f;
        bool hasMirror   = mirror.r   > 0.f || mirror.g   > 0.f || mirror.b   > 0.f;

        if(hasMirror && !hasDiffuse)
            bsdf->add(std::make_shared<SpecularReflection>(mirror));
        else
        {
            if(hasDiffuse)  
                bsdf->add(std::make_shared<LambertianReflection>(diffuse));
            if(hasSpecular && glossiness > 0.f) 
                bsdf->add(std::make_shared<BlinnPhongSpecular>(specular, glossiness));
        }
    
        
        return bsdf;
    }

};
