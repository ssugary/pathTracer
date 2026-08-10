#pragma once

#ifndef SINGLE_COLOR_BACKGROUND_HPP
#define SINGLE_COLOR_BACKGROUND_HPP

#include "Background.hpp"

namespace Prim 
{
    class SingleColorBackground : public Background
    {
        private:
            Color color;
        public:

            SingleColorBackground(const Color& color) : color(color) {}
            virtual Color sample(const Vec3&, float, float) const override
            {
                return color;
            } 
    };
} //< namespace Prim

#endif //< SINGLE_COLOR_BACKGROUND_HPP