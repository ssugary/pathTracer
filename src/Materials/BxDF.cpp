#include "BxDF.hpp"

namespace Mat 
{
    Color BxDF::sampleF(const Vec3& wo, const Normal3& n, const Point2& u,
                                Vec3* wi, float* pdf) const
    {
        Vec3 local = cosineSampleHemisphere(u);
        *wi = alignToNormal(local, n);
        *pdf = this->pdf(wo, *wi, n);
        if(*pdf <= 0.f) 
            return Color();

        return f(wo, *wi, n);
    }
    float BxDF::pdf(const Vec3&, const Vec3& wi, const Normal3& n) const
    {
        float cosTheta = dot(n, wi);
        return cosTheta > 0.f ? cosTheta / PI : 0.f;
    }
    bool BxDF::matchesFlags(BxDFType t) const 
    { 
        return (type & t) == type; 
    }

    ScaledBxDF::ScaledBxDF(BxDF* bxdf, const Color& scale)
    : bxdf(bxdf), scale(scale) 
    {
        type = bxdf->type;
    };
            
    Color ScaledBxDF::f(const Vec3& wo, const Vec3& wi, const Normal3& n) const
    {
        return scale * bxdf->f(wo, wi, n);
    }
    // Color ScaledBxDF::rho(int nSamples, const Point2* s1, const Point2* s2)   const
    // {
    //     return scale * bxdf->rho(nSamples, s1, s2);
    // }
    // Color ScaledBxDF::rho(const Vec3& wo, const Point2* s1, const Point2* s2) const 
    // {
    //     return scale * bxdf->rho(wo, s1, s2);
    // }
    Color ScaledBxDF::sampleF(const Vec3& wo, const Normal3& n, const Point2& u, Vec3* wi, float* pdf) const 
    {
        return scale * bxdf->sampleF(wo, n, u, wi, pdf);
    }
    LambertianReflection::LambertianReflection(const Color& kd) 
    : kd(kd) 
    { 
        type = BxDFType(BSDF_REFLECTION | BSDF_DIFFUSE); 
    }

    Color LambertianReflection::f(const Vec3& wo, const Vec3& wi, const Normal3& n) const
    {
        if(dot(n, wo) <= 0.f) 
            return Color();

        return kd * std::max(0.f, dot(n, wi));
    }
    // Color LambertianReflection::rho(int, const Point2*, const Point2*)   const
    // {
    //     return kd;
    // }
    // Color LambertianReflection::rho(const Vec3&, const Point2*, const Point2*) const
    // {
    //     return kd;
    // }

    BlinnPhongSpecular::BlinnPhongSpecular(const Color& ks, float glossiness) 
    : ks(ks), glossiness(glossiness) 
    { 
        type = BxDFType(BSDF_REFLECTION | BSDF_GLOSSY); 
    }

    Color BlinnPhongSpecular::f(const Vec3& wo, const Vec3& wi, const Normal3& n) const
    {
        float cosI = dot(n, wi);
        float cosO = dot(n, wo);

        if(cosI <= 0.f || cosO <= 0.f) 
            return Color();

        Vec3 hSum = wo + wi;

        if(sqrLength(hSum) <= EPSILON_6) 
            return Color();

        Vec3 h = normalize(hSum);

        if(glossiness == 0.f)
            return Color();

        return ks * std::pow(std::max(0.f, dot(n, h)), glossiness);
    }

    Color BlinnPhongSpecular::sampleF(const Vec3& wo, const Normal3& n, const Point2& u,
                                            Vec3* wi, float* pdf) const
    {
        float cosThetaH = std::pow(u.x, 1.f / (glossiness + 1.f));
        float sinThetaH = std::sqrt(std::max(0.f, 1.f - cosThetaH * cosThetaH));
        float phi = 2.f * PI * u.y;

        Vec3 hLocal(sinThetaH * std::cos(phi), sinThetaH * std::sin(phi), cosThetaH);
        Vec3 h = alignToNormal(hLocal, n);

        *wi = 2.f * dot(wo, h) * h - wo;   

        if(dot(n, *wi) <= 0.f) 
        { 
            *pdf = 0.f; 
            return Color(); 
        }

        float pdfH = (glossiness + 1.f) * std::pow(std::max(0.f, dot(n, h)), glossiness) / (2.f * PI);
        float dotOH = dot(wo, h);
        if(dotOH < EPSILON_3) 
        { 
            *pdf = 0.f; 
            return Color(); 
        }
        *pdf = (dotOH > 0.f) ? pdfH / (4.f * dotOH) : 0.f;

        if(*pdf <= 0.f) 
            return Color();
        return f(wo, *wi, n);
    }

    float BlinnPhongSpecular::pdf(const Vec3& wo, const Vec3& wi, const Normal3& n) const
    {
        if(dot(n, wi) <= 0.f || dot(n, wo) <= 0.f) 
            return 0.f;

        Vec3 hSum = wo + wi;
        
        if(sqrLength(hSum) <= EPSILON_6) 
            return 0.f;
        Vec3 h = normalize(hSum);
        float dotOH = dot(wo, h);
        
        if(dotOH <= 0.f) 
            return 0.f;
        
        float pdfH = (glossiness + 1.f) * std::pow(std::max(0.f, dot(n, h)), glossiness) / (2.f * PI);
        return pdfH / (4.f * dotOH);
    }

    SpecularReflection::SpecularReflection(const Color& kr, Fresnel* fresnel) 
    : kr(kr), fresnel(fresnel) 
    { 
        type = BxDFType(BSDF_REFLECTION | BSDF_SPECULAR); 
    }

    Color SpecularReflection::f(const Vec3&, const Vec3&, const Normal3&) const 
    { 
        return Color(); 
    }
    float SpecularReflection::pdf(const Vec3&, const Vec3&, const Normal3&) const 
    { 
        return 0.f; 
    }
    Color SpecularReflection::sampleF(const Vec3& wo, const Normal3& n, const Point2&,
                    Vec3* wi, float* pdf) const
    {
        *pdf = 1.f;
        *wi = -wo + 2.f * dot(wo, n) * n;
        float cosTheta = std::abs(dot(n, *wi));

        if(fresnel)
            return fresnel->evaluate(cosTheta) * kr / absCosTheta(*wi);

        return cosTheta > 0.f ? kr / cosTheta : Color();
    }

    FlatBxDF::FlatBxDF(const Color& color)
    : color(color) {};

    Color FlatBxDF::f(const Vec3&, const Vec3&, const Normal3&) const 
    {
        return color;
    }
};