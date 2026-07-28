#ifndef IMAGE_BACKGROUND_HPP
#define IMAGE_BACKGROUND_HPP

#include <Geometry/Backgrounds/Background.hpp>
#include <STBI/stb_image.h>

namespace Prim 
{
    class ImageBackground : public Background
    {
        private:

            unsigned char* data{nullptr};
            int width{0};
            int height{0};

        public:
        
            ImageBackground(unsigned char* data, const int w, const int h)
            : data(std::move(data)), width(w), height(h) {};

            ~ImageBackground()
            {
                if(data)
                    stbi_image_free(data);
            }

            Color sample(const Vec3& d, float=1.f, float=1.f) const override;

    };

}; //< namespace Prim


#endif //< IMAGE_BACKGROUND_HPP