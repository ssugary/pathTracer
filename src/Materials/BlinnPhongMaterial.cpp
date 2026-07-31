#include "BlinnPhongMaterial.hpp"

namespace Mat 
{

    Color BlinnPhongMaterial::f(const Vec3& wo, const Vec3& wi, const Normal3& n) const
    {
        Vec3 h = normalize(wo + wi);

        Color diffuseColor = diffuse;

        Color spec{0};

        if(dot(n, wi) > 0.f && glossiness != 0)
        {
            spec = specular * std::pow(std::max(0.f, dot(n, h)), glossiness);
        }

        return diffuseColor + spec;
    }
    Color BlinnPhongMaterial::sampleF(const Vec3& wo, const Normal3& n, const Point2& u,
                                    Vec3* wi, float* pdf) const 
    {
        Vec3 local = cosineSampleHemisphere(u);
        *wi = alignToNormal(local, n);
        *pdf = std::max(0.f, dot(n, *wi)) / PI;
        if(*pdf <= 0.f)
            return Color();

        return f(wo, *wi, n);
    }

};
