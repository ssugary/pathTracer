#ifndef AMBIENT_LIGHT_HPP
#define AMBIENT_LIGHT_HPP

#include <Light/Light.hpp>
namespace Luz 
{
    class AmbientLight : public Light
    {
        public:

            AmbientLight(const Color& intensity, const Color& scale) 
            : Light(intensity, scale)
            {
                flag = LightFlag::AMBIENT;
            }
            Color sampleLi(const Geo::Interaction&, 
                                    const Point2&, 
                                    Vec3* wi, float* pdf, VisibilityTester*) const override
            {
                *wi = {0, 0, 0};
                *pdf = 1.0f;
                return intensity * scale;
            }
            float pdf(const Geo::Interaction&, const Vec3&) const override
            {
                return 0.f;
            }
    };
};


#endif //< AMBIENT_LIGHT_HPP