#pragma once

#ifndef IMAGE_BACKGROUND_HPP
#define IMAGE_BACKGROUND_HPP

#include <Geometry/Backgrounds/Background.hpp>
#include <memory>
#include <STBI/stb_image.h>

namespace Prim 
{
    class ImageBackground : public Background
    {
        private:

            std::shared_ptr<unsigned char> data{nullptr};
            int width{0};
            int height{0};

        public:
        
            ImageBackground(std::shared_ptr<unsigned char> data, const int w, const int h)
            : data(std::move(data)), width(w), height(h) {};

            ~ImageBackground() = default;

            Color sample(const Vec3& d, float=1.f, float=1.f) const override;

    };

}; //< namespace Prim


#endif //< IMAGE_BACKGROUND_HPP