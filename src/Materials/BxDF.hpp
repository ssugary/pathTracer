#pragma once 

#ifndef BXDF_HPP
#define BXDF_HPP

#include "Utils/common.hpp"
#include "Fresnel.hpp"

namespace Mat 
{
    enum BxDFType 
    {
        BSDF_REFLECTION   = 1 << 0,
        BSDF_TRANSMISSION = 1 << 1,
        BSDF_DIFFUSE      = 1 << 2,
        BSDF_GLOSSY       = 1 << 3,
        BSDF_SPECULAR     = 1 << 4,
        BSDF_ALL          = BSDF_REFLECTION | BSDF_DIFFUSE | BSDF_GLOSSY | BSDF_SPECULAR | BSDF_TRANSMISSION
    };

    class BxDF 
    {
        public:
            BxDFType type;
            virtual Color f(const Vec3& wo, const Vec3& wi, const Normal3& n) const = 0;
            virtual Color sampleF(const Vec3& wo, const Normal3& n, const Point2& u,
                                Vec3* wi, float* pdf) const; 
            virtual float pdf(const Vec3& wo, const Vec3& wi, const Normal3& n)   const;
            // virtual Color rho(int, const Point2*, const Point2*)   const {};
            // virtual Color rho(const Vec3&, const Point2*, const Point2*) const {};
            bool matchesFlags(BxDFType t) const;
    };

    class ScaledBxDF : public BxDF 
    {
        private:
            BxDF* bxdf;
            Color scale;
        public:
            ScaledBxDF(BxDF* bxdf, const Color& scale);

            Color f(const Vec3& wo, const Vec3& wi, const Normal3& n) const override;
            // Color rho(int nSamples, const Point2* s1, const Point2* s2)   const override;
            // Color rho(const Vec3& wo, const Point2* s1, const Point2* s2) const override;
            Color sampleF(const Vec3& wo, const Normal3& n, const Point2& u, Vec3* wi, float* pdf) const override; 
    };

    class LambertianReflection : public BxDF 
    {
        private:
            Color kd;
        public: 
            LambertianReflection(const Color& kd);

            Color f(const Vec3& wo, const Vec3& wi, const Normal3& n) const override;
            // Color rho(int nSamples, const Point2* s1, const Point2* s2)   const override;
            // Color rho(const Vec3& wo, const Point2* s1, const Point2* s2) const override;
    };
    class BlinnPhongSpecular   : public BxDF 
    {
        private:
            Color ks;
            float glossiness;
        public:

            BlinnPhongSpecular(const Color& ks, float glossiness);

            Color f(const Vec3& wo, const Vec3& wi, const Normal3& n) const override;
            Color sampleF(const Vec3& wo, const Normal3& n, const Point2& u,
                          Vec3* wi, float* pdf) const override;
            float pdf(const Vec3& wo, const Vec3& wi, const Normal3& n) const override;
    };

    class SpecularReflection   : public BxDF 
    {  
        private:
            Color kr;
            Fresnel* fresnel;
        public:

            SpecularReflection(const Color& kr, Fresnel* fresnel=nullptr);

            Color f(const Vec3&, const Vec3&, const Normal3&) const override; 
            float pdf(const Vec3&, const Vec3&, const Normal3&) const override;
            Color sampleF(const Vec3& wo, const Normal3& n, const Point2&,
                          Vec3* wi, float* pdf) const override;
    };

    class FlatBxDF : public BxDF 
    {
        private:
            Color color;
        public:
            FlatBxDF(const Color& color);
            Color f(const Vec3&, const Vec3&, const Normal3&) const override; 
    };

}


#endif //< BXDF_HPP