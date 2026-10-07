#pragma once

#ifndef MICROFACETS_HPP
#define MICROFACETS_HPP

#include "BxDF.hpp"
#include <memory>

namespace Mat 
{
    class MicrofacetDistribution
    {
        protected:

            const bool visibleArea;
            virtual float lambda(const Vec3& w) const = 0;

        public:

            MicrofacetDistribution(bool visibleArea);            
            virtual float D(const Vec3& wh) const = 0;
            virtual Vec3 sampleWH(const Vec3& wo, const Point2& u) const = 0;
            float pdf(const Vec3& wo, const Vec3& wh) const;
            float G1(const Vec3& w) const;
            float G(const Vec3& wo, const Vec3& wi) const;
    };

    class BeckmannDistribution : public MicrofacetDistribution
    {
        private:

            const float alphaX;
            const float alphaY; 

            float lambda(const Vec3& w) const override;
        
        public:
            BeckmannDistribution(float alphaX, float alphaY, bool vis);
            float D(const Vec3& wh) const override;
            Vec3 sampleWH(const Vec3& wo, const Point2& u) const override;
    };

    class TrowbridgeReitzDistribution : public MicrofacetDistribution
    {
        private:

            const float alphaX;
            const float alphaY; 

            float lambda(const Vec3& w) const override;
        
        public:
            TrowbridgeReitzDistribution(float alphaX, float alphaY, bool vis);
            float D(const Vec3& wh) const override;
            Vec3 sampleWH(const Vec3& wo, const Point2& u) const override;
    };

    class MicrofacetReflection : public BxDF 
    {
        private:

            Color kr;
            std::shared_ptr<MicrofacetDistribution> distribution;
            float etaA;
            float etaB;
            std::shared_ptr<Fresnel> fresnel;

        public:
            MicrofacetReflection(const Color& kr, std::shared_ptr<MicrofacetDistribution> distribution, float etaA, float etaB, std::shared_ptr<Fresnel> fresnel);

            Color f(const Vec3& wo, const Vec3& wi, const Normal3& n) const override;
            Color sampleF(const Vec3& wo, const Normal3& n, const Point2& u,
                                     Vec3* wi, float* pdf) const override;
            float pdf(const Vec3& wo, const Vec3& wi, const Normal3& n) const override;

    };

    class MicrofacetTransmission : public BxDF 
    {
        private:

            Color kr;
            std::shared_ptr<MicrofacetDistribution> distribution;
            float etaA;
            float etaB;
            std::shared_ptr<Fresnel> fresnel;

        public:
            MicrofacetTransmission(const Color& kr, std::shared_ptr<MicrofacetDistribution> distribution, float etaA, float etaB, std::shared_ptr<Fresnel> fresnel);

            Color f(const Vec3& wo, const Vec3& wi, const Normal3& n) const override;
            Color sampleF(const Vec3& wo, const Normal3& n, const Point2& u,
                                     Vec3* wi, float* pdf) const override;
            float pdf(const Vec3& wo, const Vec3& wi, const Normal3& n) const override;
    };
};

#endif //< MICROFACETS_HPP