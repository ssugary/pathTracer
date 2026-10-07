#ifndef ENVIRONMENT_LIGHT_HPP
#define ENVIRONMENT_LIGHT_HPP

#include "Light.hpp"
#include "Utils/common.hpp"
#include "VisibilityTester.hpp"

namespace Luz 
{
    class EnvironmentLight : public Light 
    {
         private:
            std::shared_ptr<unsigned char> data;
            int width, height;

        public:

            EnvironmentLight(std::shared_ptr<unsigned char> imgData, int w, int h, const Color& scale)
            : Light(Color(1.f), scale), data(imgData), width(w), height(h)
            {
                flag = LightFlag::ENVIRONMENT;  
            }

            Color lookup(const Vec3& d) const
            {
                float theta = std::acos(std::clamp(d.y, -1.0f, 1.0f));
                float phi = std::atan2(d.z, d.x);
                if (phi < 0.0f) phi += 2.0f * PI;

                float u = phi / (2.0f * PI);
                float v = theta / PI;

                int x = std::clamp(static_cast<int>(u * width), 0, width - 1);
                int y = std::clamp(static_cast<int>(v * height), 0, height - 1);
                int index = (y * width + x) * 3;

                return Color(data.get()[index]/255.f, data.get()[index+1]/255.f, data.get()[index+2]/255.f) * scale;
            }

            Color sampleLi(const Geo::Interaction& ref, const Point2& u, 
                            Vec3* wi, float* pdf, VisibilityTester* vis) const override
            {
                Vec3 local = cosineSampleHemisphere(u);
                *wi = alignToNormal(local, ref.n);
                *pdf = std::max(0.f, dot(ref.n, *wi)) / PI;

                if(*pdf <= 0.f) return Color();

                Interaction lpos;
                lpos.p = ref.p + (*wi) * 1e6f;   
                *vis = VisibilityTester(ref, lpos);

                return lookup(*wi);
            }

            float pdf(const Geo::Interaction& ref, const Vec3& wi) const override
            {
                float cosTheta = dot(ref.n, wi);
                return cosTheta > 0.f ? cosTheta / PI : 0.f;
            }
    };
}


#endif //< ENVIRONMENT_LIGHT_HPP