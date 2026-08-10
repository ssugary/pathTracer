#include "BlinnPhongMaterial.hpp"

namespace Mat 
{

    SampledSpectrum BlinnPhongMaterial::f(const Vec3& wo, const Vec3& wi, const Normal3& n, const ssrt::SampledWavelengths& lambdas) const
    {
        Vec3 h = normalize(wo + wi);

        SampledSpectrum diffuseColor{0.f};
    
        if(diffuse) 
            diffuseColor = diffuse->sample(lambdas) / PI;
        
        SampledSpectrum spec{0.f};

        if(dot(n, wi) > 0.f && glossiness > 0.f)
        {
            spec = specular->sample(lambdas) * std::pow(std::max(0.f, dot(n, h)), glossiness);
        }

        return diffuseColor + spec;

    }
    SampledSpectrum BlinnPhongMaterial::sampleF(const Vec3& wo, const Normal3& n, const Point2& u, const ssrt::SampledWavelengths& lambdas,
                                                Vec3* wi, float* pdf) const 
    {
        SampledSpectrum m{0.f};
        if(mirror) 
            m = mirror->sample(lambdas);

        bool isMirror = (bool)m;

        if (isMirror)
        {
            *wi = normalize(-wo + 2.f * dot(wo, n) * n);
            
            float cosThetaI = dot(n, *wi);

            if (cosThetaI <= SHADOW_EPSILON) 
            {
                *pdf = 0.f;
                return SampledSpectrum(0.f); 
            }

            *pdf = 1.0f;

            return m / cosThetaI;
        }
        
        Vec3 local = cosineSampleHemisphere(u);
        *wi = alignToNormal(local, n);
        *pdf = std::max(0.f, dot(n, *wi)) / PI;

        if(*pdf <= SHADOW_EPSILON)
            return SampledSpectrum(0.f);

        return f(wo, *wi, n, lambdas);
    }

};
