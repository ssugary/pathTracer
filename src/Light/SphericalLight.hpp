#ifndef SPHERICAL_AREA_LIGHT_HPP
#define SPHERICAL_AREA_LIGHT_HPP

#include "AreaLight.hpp"
#include "VisibilityTester.hpp"


namespace Luz 
{
    class SphericalLight : public AreaLight
    {
        private:

            Point3 center;
            float radius;
            float area;
        
        public:
        
            SphericalLight(const Color& intensity, const Color& scale, const Point3& center, float radius)
            : AreaLight(intensity, scale), center(center), radius(radius)
            {
                flag = LightFlag::AREA;
                area = 4.f * PI * radius * radius;
            }

            Color L(const Geo::Interaction& intr, const Vec3& w) const override
            {
                return dot(intr.n, w) > 0 ? intensity * scale : Color(0.f, 0.f, 0.f);
            }

            std::shared_ptr<AreaLight> clone() const override
            {
                std::shared_ptr<AreaLight> cloned = std::make_shared<SphericalLight>(intensity, scale, center, radius);
                
                cloned->shape = this->shape;
                cloned->O2W = this->O2W;
                cloned->W2O = this->W2O;

                return cloned;
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

                return L(pShape, -*wi);
            }

    };
};

#endif //< SPHERICAL_AREA_LIGHT_HPP