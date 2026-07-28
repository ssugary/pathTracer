#ifndef POINT_LIGHT_HPP
#define POINT_LIGHT_HPP

#include "Light.hpp"
#include "VisibilityTester.hpp"

namespace Luz 
{

    class PointLight : public Light
    {
        private:

            Point3 pos;
            Vec3 attenuation;

        public:

            PointLight(Color intensity, Color scale, Point3 pos, Vec3 attenuation)
            : Light(intensity, scale), pos(pos), attenuation(attenuation)
            {
                flag = LightFlag::POINT;
            };

            Color sampleLi(const Geo::Interaction& hit, 
                            // const Point2& u, 
                            Vec3* wi, VisibilityTester* vis) const override
            {
               Vec3 dir = pos - hit.p;
               *wi = normalize(dir);
               Geo::Interaction lpos; lpos.p = pos;
               *vis = VisibilityTester(hit, lpos);

               float dist = length(dir);

               float att = 1.0f / (attenuation[0] + dist * attenuation[1] + dist * dist * attenuation[2]);

               return intensity * scale * att;

            }
    };

}

#endif //< POINT_LIGHT_HPP