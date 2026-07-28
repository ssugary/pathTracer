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
                            // const Point2& u, 
                            Vec3* wi, VisibilityTester*) const override
            {
                *wi = {0, 0, 0};
                return intensity * scale;
            }
    };
};


#endif //< AMBIENT_LIGHT_HPP