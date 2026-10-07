#ifndef SSMATH_PIXEL_HPP
#define SSMATH_PIXEL_HPP

#include "vector.hpp"
#include "color.hpp"
#include <atomic>

namespace ssmath3
{

    struct Pixel 
    {
        Color colorSum{0.f, 0.f, 0.f};
        float filterWeightSum{0.f};
        float colorVariance{0.f};
        
        Color albedoSum{0.f, 0.f, 0.f};
        Vector<float, 3> normalSum{0.f, 0.f, 0.f};
        float depthSum{0.f};

        void reset() {
            colorSum = Color(0.f);
            albedoSum = Color(0.f);
            normalSum = Vector<float, 3>(0.f, 0.f, 0.f);
            filterWeightSum = 0.f;
            colorVariance = 0.f;
            depthSum = 0.f;
        }
    
    
    };

};


#endif
