#include "ImageBackground.hpp"
#include "Geometry/Rays/Ray.hpp"

namespace Prim 
{

    Color ImageBackground::sample(const Vec3& d, float, float) const
    {
        if(!data)
            return Color();

        float theta = std::acos(std::clamp(d.y, -1.0f, 1.0f)); 
        float phi = std::atan2(d.z, d.x);

        if (phi < 0.0f) 
            phi += 2.0f * PI; 
        

        float u = phi / (2.0f * PI);
        float v = theta / PI;

        int x = std::clamp(static_cast<int>(u * width), 0, width - 1);
        int y = std::clamp(static_cast<int>(v * height), 0, height - 1);

        int index = (y * width + x) * 3;

        unsigned char r = data[index + 0];
        unsigned char g = data[index + 1];
        unsigned char b = data[index + 2];

        return Color(r/255.f, g/255.f,b/255.f);
    };

};