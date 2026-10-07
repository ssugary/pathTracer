#include "Microfacets.hpp"

namespace Mat 
{

    MicrofacetDistribution::MicrofacetDistribution(bool visibleArea) 
    : visibleArea(visibleArea) {};

    float MicrofacetDistribution::G1(const Vec3& w) const 
    {
        return 1.f/(1.f + lambda(w));
    }

    float MicrofacetDistribution::G(const Vec3& wo, const Vec3& wi) const
    {
        return 1.f / (1.f + lambda(wo) + lambda(wi));
    }

    float MicrofacetDistribution::pdf(const Vec3& wo, const Vec3& wh) const
    {
        if (visibleArea)
            return D(wh) * G1(wo) * std::abs(dot(wo, wh)) / absCosTheta(wo);

        return D(wh) * absCosTheta(wh);
    }

    BeckmannDistribution::BeckmannDistribution(float alphaX, float alphaY, bool vis)
    : MicrofacetDistribution(vis), alphaX(alphaX), alphaY(alphaY) {}

    float BeckmannDistribution::D(const Vec3& wh) const 
    {
        float tan2T = tan2Theta(wh);
        if(std::isinf(tan2T))
            return 0.f;

        return std::exp(-tan2T * (cos2Phi(wh) / (alphaX * alphaX) + sin2Phi(wh) / (alphaY * alphaY))) 
                  / (PI * alphaX * alphaY * cos2Theta(wh) * cos2Theta(wh));
    }

    float BeckmannDistribution::lambda(const Vec3& w) const
    {
        float absTanT = std::abs(tanTheta(w));
        if(std::isinf(absTanT))
            return 0.f;

        float alpha = std::sqrt(cos2Phi(w) * alphaX * alphaX + sin2Phi(w) * alphaY * alphaY);

        float a = 1.f / (alpha * absTanT);

        if(a >= 1.6f)
            return 0.f;

        return (1 - 1.256f * a + 0.396f * a * a) / (3.535f * a + 2.181f * a * a); //< Beckmann–Spizzichino approximation
    }

    Vec3 BeckmannDistribution::sampleWH(const Vec3& wo, const Point2& u) const
    {
        float tan2Theta;
        float phi;
        float logSample = std::log(1.f - u.x);

        if (std::isinf(logSample)) 
                logSample = 0.f;
        
        if (alphaX == alphaY) //< caso fodase
        {
            tan2Theta = -alphaX * alphaX * logSample;
            phi = u.y * 2.f * PI;
        }
        else
        {
            phi = std::atan(alphaY / alphaX * std::tan(2.f * PI * u.y + 0.5f * PI));
            if (u.y > 0.5f) 
                phi += PI;
            float sinPhi = std::sin(phi);
            float cosPhi = std::cos(phi);
            float alphax2 = alphaX * alphaX;
            float alphay2 = alphaY * alphaY;
            tan2Theta = -logSample / (cosPhi * cosPhi / alphax2 + sinPhi * sinPhi / alphay2);
        }

        float cosTheta = 1.f / std::sqrt(1.f + tan2Theta);
        float sinTheta = std::sqrt(std::max(0.f, 1.f - cosTheta * cosTheta));

        Vec3 wh = sphericalDirection(sinTheta, cosTheta, phi);
        if (wo.z * wh.z < 0.f) 
            wh = -wh;   
        return wh;
    }

    TrowbridgeReitzDistribution::TrowbridgeReitzDistribution(float alphaX, float alphaY, bool vis)
    : MicrofacetDistribution(vis), alphaX(alphaX), alphaY(alphaY) {}

    float TrowbridgeReitzDistribution::D(const Vec3& wh) const 
    {
        float tan2T = tan2Theta(wh);
        if(std::isinf(tan2T))
            return 0.f;

        float e = tan2T * (cos2Phi(wh) / (alphaX * alphaX) + sin2Phi(wh) / (alphaY * alphaY));

        return 1.f / (PI * alphaX * alphaY * cos2Theta(wh) * cos2Theta(wh) * (1.f + e) * (1.f + e));
    }

    float TrowbridgeReitzDistribution::lambda(const Vec3& w) const
    {
        float absTanT = std::abs(tanTheta(w));
        if(std::isinf(absTanT))
            return 0.f;

        float alpha2 = cos2Phi(w) * alphaX * alphaX + sin2Phi(w) * alphaY * alphaY;

        return (-1.f + std::sqrt(1.f + (alpha2 * absTanT * absTanT))) / 2.f;
    }
    Vec3 TrowbridgeReitzDistribution::sampleWH(const Vec3& wo, const Point2& u) const
    {

        Vec3 w = ::normalize(Vec3(alphaX * wo.x, alphaY * wo.y, wo.z));
        if(w.z < 0.f)
            w = -w;

        float s = 0.5f * (1.f + w.z);

        Vec3 t1 = (w.z < 1.f) ? normalize(cross(Vec3(0,0,1), w)): Vec3(1,0,0);

        Vec3 t2 = cross(w, t1);


        Point2 p = concentricSampleDisk(u);
        
        float p1 = p.x;
        float p2 = (1.f - s) * std::sqrt(std::max(0.f, 1.f - p1 * p1)) + s * p.y;
        
        Vec3 N = p1 * t1 + p2 * t2 + std::sqrt(std::max(0.f, 1.f - p1 * p1 - p2 * p2)) * w;

        Vec3 wh = normalize(Vec3(alphaX * N.x, alphaY * N.y, std::max(0.f, N.z)));

        if (wo.z * wh.z < 0.f) 
            wh = -wh;

        return wh;
    }

    MicrofacetReflection::MicrofacetReflection(const Color& kr, std::shared_ptr<MicrofacetDistribution> distribution, float etaA, float etaB, std::shared_ptr<Fresnel> fresnel)
    : kr(kr), distribution(distribution), etaA(etaA), etaB(etaB), fresnel(fresnel) 
    {
        type = BxDFType(BSDF_GLOSSY | BSDF_REFLECTION);
    };

    float MicrofacetReflection::pdf(const Vec3& wo, const Vec3& wi, const Normal3& n) const
    {
        Vec3 t1;
        Vec3 t2;
        coordinateSystem(static_cast<Vec3>(n), &t1, &t2);
        Vec3 woLocal(dot(wo, t1), dot(wo, t2), dot(wo, static_cast<Vec3>(n)));
        Vec3 wiLocal(dot(wi, t1), dot(wi, t2), dot(wi, static_cast<Vec3>(n)));

        if (!sameHemisphere(woLocal, wiLocal)) 
            return 0.f;

        Vec3 wh = normalize(woLocal + wiLocal);

        return distribution->pdf(woLocal, wh) / (4.f * dot(woLocal, wh));
    }

    Color MicrofacetReflection::f(const Vec3& wo, const Vec3& wi, const Normal3& n) const 
    {
        Vec3 t1;
        Vec3 t2;
        coordinateSystem(static_cast<Vec3>(n), &t1, &t2);
        Vec3 woLocal(dot(wo, t1), dot(wo, t2), dot(wo, static_cast<Vec3>(n)));
        Vec3 wiLocal(dot(wi, t1), dot(wi, t2), dot(wi, static_cast<Vec3>(n)));

        float cosTO = absCosTheta(woLocal);
        float cosTI = absCosTheta(wiLocal);
        
        if (cosTheta(woLocal) * cosTheta(wiLocal) <= 0.f || cosTO == 0.f || cosTI == 0.f)
            return Color(0.f);
        Vec3 wh = woLocal + wiLocal; 

        if (sqrLength(wh) <= EPSILON_6)
            return Color(0.f);
        
        wh = normalize(wh);

        Color F = fresnel ? fresnel->evaluate(dot(wiLocal, wh)) : Color(1.f);

        return distribution->D(wh) * distribution->G(woLocal, wiLocal) * F * kr / (4.f * cosTI * cosTO);

    }
    Color MicrofacetReflection::sampleF(const Vec3& wo, const Normal3& n, const Point2& u,
                                        Vec3* wi, float* pdf) const 
    {
        Vec3 t1;
        Vec3 t2;
        coordinateSystem(static_cast<Vec3>(n), &t1, &t2);
        Vec3 woLocal(dot(wo, t1), dot(wo, t2), dot(wo, static_cast<Vec3>(n)));

        if (woLocal.z == 0.f) 
        { 
            *pdf = 0.f; 
            return Color(0.f); 
        }

        Vec3 whLocal = distribution->sampleWH(woLocal, u);

        if (dot(woLocal, whLocal) < 0.f) 
        { 
            *pdf = 0.f; 
            return Color(0.f); 
        }

        Vec3 wiLocal = -woLocal + 2.f * dot(woLocal, whLocal) * whLocal;

        if (woLocal.z * wiLocal.z <= 0.f) 
        { 
            *pdf = 0.f; 
            return Color(0.f); 
        }  

        *wi = wiLocal.x * t1 + wiLocal.y * t2 + wiLocal.z * static_cast<Vec3>(n);

        *pdf = distribution->pdf(woLocal, whLocal) / (4.f * dot(woLocal, whLocal));

        if (*pdf <= 0.f) 
            return Color(0.f);

        return f(wo, *wi, n);
    }

    MicrofacetTransmission::MicrofacetTransmission(const Color& kr, std::shared_ptr<MicrofacetDistribution> distribution, float etaA, float etaB, std::shared_ptr<Fresnel> fresnel)
    : kr(kr), distribution(distribution), etaA(etaA), etaB(etaB), fresnel(fresnel) 
    {
        type = BxDFType(BSDF_TRANSMISSION | BSDF_GLOSSY);
    };

    float MicrofacetTransmission::pdf(const Vec3& wo, const Vec3& wi, const Normal3& n) const
    {
        Vec3 t1;
        Vec3 t2;
        coordinateSystem(static_cast<Vec3>(n), &t1, &t2);
        Vec3 woLocal(dot(wo, t1), dot(wo, t2), dot(wo, static_cast<Vec3>(n)));
        Vec3 wiLocal(dot(wi, t1), dot(wi, t2), dot(wi, static_cast<Vec3>(n)));

        if (cosTheta(woLocal) * cosTheta(wiLocal) > 0.f) 
            return 0.f;   

        float eta = cosTheta(woLocal) > 0.f ? (etaB / etaA) : (etaA / etaB);
        Vec3 wh = normalize(woLocal + wiLocal * eta);

        if (woLocal.z * wh.z < 0.f) 
            wh = -wh;

        if (dot(woLocal, wh) * dot(wiLocal, wh) > 0.f)
            return 0.f;

        float sqrtDenom = dot(woLocal, wh) + eta * dot(wiLocal, wh);
        float dwh_dwi = std::abs((eta * eta * dot(wiLocal, wh)) / (sqrtDenom * sqrtDenom));

        return distribution->pdf(woLocal, wh) * dwh_dwi;
    }

    Color MicrofacetTransmission::f(const Vec3& wo, const Vec3& wi, const Normal3& n) const 
    {

        Vec3 t1;
        Vec3 t2;
        coordinateSystem(static_cast<Vec3>(n), &t1, &t2);
        Vec3 woLocal(dot(wo, t1), dot(wo, t2), dot(wo, static_cast<Vec3>(n)));
        Vec3 wiLocal(dot(wi, t1), dot(wi, t2), dot(wi, static_cast<Vec3>(n)));

        float cosTO = absCosTheta(woLocal);
        float cosTI = absCosTheta(wiLocal);


        if(cosTO == 0.f || cosTI == 0.f)
            return Color(0.f);

        float eta = cosTheta(woLocal) > 0 ? (etaB / etaA) : (etaA / etaB);
        

        Vec3 wh = normalize(woLocal + wiLocal * eta);

        if(wh.z < 0)
            wh = -wh;
        
        if(dot(woLocal, wh) * dot(wiLocal, wh) > 0.f)
            return Color(0.f);

        float sqrtd = dot(woLocal, wh) + eta * dot(wiLocal, wh);

        return kr * (Color(1.f) - fresnel->evaluate(dot(wiLocal, wh))) 
                * std::abs(
                    distribution->D(wh) * distribution->G(woLocal, wiLocal)  
                       * eta * eta * std::abs(dot(wiLocal, wh)) * std::abs(dot(woLocal, wh))
                       / (cosTI * cosTO * sqrtd * sqrtd)
                          );

    }
    Color MicrofacetTransmission::sampleF(const Vec3& wo, const Normal3& n, const Point2& u,
                          Vec3* wi, float* pdf) const 
    {
        Vec3 t1;
        Vec3 t2;
        coordinateSystem(static_cast<Vec3>(n), &t1, &t2);
        Vec3 woLocal(dot(wo, t1), dot(wo, t2), dot(wo, static_cast<Vec3>(n)));

        if (woLocal.z == 0.f) 
        { 
            *pdf = 0.f; 
            return Color(0.f); 
        }

        Vec3 whLocal = distribution->sampleWH(woLocal, u);
        if (dot(woLocal, whLocal) < 0.f) 
        { 
            *pdf = 0.f; 
            return Color(0.f); 
        }

        float eta = cosTheta(woLocal) > 0.f ? (etaA / etaB) : (etaB / etaA);

        Vec3 wiLocal;
        if (!refract(woLocal, Normal3(whLocal), eta, &wiLocal) || woLocal.z * wiLocal.z > 0.f)
        {
            *pdf = 0.f;
            return Color(0.f);  
        }

        *wi = wiLocal.x * t1 + wiLocal.y * t2 + wiLocal.z * static_cast<Vec3>(n);

        *pdf = this->pdf(wo, *wi, n);  

        if (*pdf <= 0.f) 
            return Color(0.f);

        return f(wo, *wi, n);
    }
            


};