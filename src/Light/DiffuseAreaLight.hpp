#ifndef DIFFUSE_AREA_LIGHT_HPP
#define DIFFUSE_AREA_LIGHT_HPP

#include "AreaLight.hpp"
#include "VisibilityTester.hpp"


namespace Luz 
{
    class DiffuseAreaLight : public AreaLight
    {
        private:

            bool twoSided;
        
        public:
        
            DiffuseAreaLight(const Color& intensity, const Color& scale, bool twoSided = false)
            : AreaLight(intensity, scale), twoSided(twoSided)
            {
                shape = nullptr;
                flag = LightFlag::AREA;
            }

            Color L(const Geo::Interaction& intr, const Vec3& w) const override
            {
                return (twoSided || dot(intr.n, w) > 0) ? intensity * scale : Color(0.f, 0.f, 0.f);
            }

            Color sampleLi(const Geo::Interaction& ref, 
                                    const Point2& u, 
                                    Vec3* wi, float* pdf, VisibilityTester* vis) const override
            {
                Geo::Interaction pShape = shape->sample(ref, u, pdf);

                if (*pdf == 0.f) 
                    return Color();

                Vec3 d = pShape.p - ref.p;

                float dist2 = sqrLength(d);

                if (dist2 == 0.f) 
                {
                    *pdf = 0.f;
                    return Color();
                }

                *wi = normalize(d);
                *vis = VisibilityTester(ref, pShape);

                float cosThetaLight = std::abs(dot(pShape.n, -*wi));

                if (cosThetaLight < EPSILON_6) 
                {
                    *pdf = 0.f;
                    return Color();
                }

                *pdf *= dist2 / cosThetaLight;

                return L(pShape, -*wi);
            }
    };
};

#endif //< DIFFUSE_AREA_LIGHT_HPP