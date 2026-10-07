#pragma once 

#ifndef FRESNEL_HPP
#define FRESNEL_HPP

#include "Utils/common.hpp"

namespace Mat 
{
    inline float FrDielectric(float cos, float etaI, float etaT) 
    {
        float cosI = std::clamp(cos, -1.f, 1.f);

        if (cosI <= 0.f) 
        {
            std::swap(etaI, etaT);
            cosI = std::abs(cosI);
        }

        float sinI = std::sqrt(std::max(0.f, 1 - cosI * cosI));
        float sinT = etaI / etaT * sinI;
        if (sinT >= 1)
            return 1;

        float cosT = std::sqrt(std::max(0.f, 1 - sinT * sinT));

        float t1 = etaT * cosI;
        float t2 = etaI * cosT;
        float t3 = etaI * cosI;
        float t4 = etaT * cosT;

        float Rparl = (t1 - t2) / (t1 + t2);
        float Rperp = (t3 - t4) / (t3 + t4);

        return (Rparl * Rparl + Rperp * Rperp) / 2;
    }

    inline Color FrConductor(float cos, const Color& etaI, const Color& etaT, const Color& k)
    {
        float cost = std::clamp(cos, -1.f, 1.f);

        Color eta = etaT / etaI;
        Color etak = k / etaI;
        Color eta2 = eta * eta;
        Color etak2 = etak * etak;

        float cosI2 = cost * cost;
        float sinI2 = 1 - cosI2;

        Color t0 = eta2 - etak2 - sinI2;
        Color a2b2 = t0 * t0 + 4.f * eta2 * etak2;
        a2b2.r = std::sqrt(a2b2.r);
        a2b2.g = std::sqrt(a2b2.g);
        a2b2.b = std::sqrt(a2b2.b);

        Color t1 = a2b2 + cosI2;
        Color a = (a2b2 + t0) / 2.f;

        a.r = std::sqrt(a.r);
        a.g = std::sqrt(a.g);
        a.b = std::sqrt(a.b);

        Color t2 = 2.f * a * cost;

        Color Rs = (t1 - t2) / (t1 + t2);

        Color t3 = cosI2 * a2b2 + sinI2 * sinI2;
        Color t4 = t2 * sinI2;
        Color Rp = Rs * (t3 - t4) / (t3 + t4);

        return (Rp + Rs) / 2;
    }

    class Fresnel 
    {
        public:
            virtual ~Fresnel() = default;
            virtual Color evaluate(float cos) const = 0;
    };

    class FresnelConductor : public Fresnel
    {
        private:

            Color etaI;
            Color etaT;
            Color k;

        public:

            FresnelConductor(const Color& etaI, const Color& etaT, const Color& k)
            : etaI(etaI), etaT(etaT), k(k) {};

            Color evaluate(float cos) const override
            {
                return FrConductor(std::abs(cos), etaI, etaT, k);
            }
    };

    class FresnelDielectric : public Fresnel 
    {
        private:

            float etaI;
            float etaT;

        public:

        FresnelDielectric(float etaI, float etaT)
        : etaI(etaI), etaT(etaT) {}

        Color evaluate(float cos) const override
        {
            return FrDielectric(cos, etaI,  etaT);
        }

    };

};

#endif //< FRESNEL_HPP