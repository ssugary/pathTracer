#pragma once

#ifndef INTERPOLED_BACKGROUND_HPP
#define INTERPOLED_BACKGROUND_HPP

#include "Background.hpp"
#include <array>

namespace Prim 
{
    class InterpoledBackground : public Background
    {
        private:

        enum Corners
        {
            bl=0, //!< Bottom left corner.
            tl,   //!< Top left corner.
            tr,   //!< Top right corner.
            br    //!< Bottom right corner.
        };
            std::array<Color, 4> colors;

        public:
            InterpoledBackground(Color bl, Color tl, Color tr, Color br) : colors({bl, tl, tr, br}) {};
            InterpoledBackground(std::array<Color, 4> colors) : colors(colors) {};
            virtual Color sample(const Vec3& d, float u=1.f, float v=1.f) const override;
    };
}; //< namespace Prim

#endif //< INTERPOLED_BACKGROUND_HPP