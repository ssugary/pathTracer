#pragma once

#ifndef OCTAEDRAL_VECTOR_HPP
#define OCTAEDRAL_VECTOR_HPP

#include "ssmath3/ssmath3.hpp"
#include <cmath>
#include <cstdint>
using namespace ssmath3;
namespace ssrt 
{
    class OctaedralVector 
    {
        private:

            uint16_t x;
            uint16_t y;

            inline uint16_t encode(float f) const
            {
                float n = std::clamp(f, -1.f, 1.f);
                return static_cast<uint16_t>(std::round((n + 1)/2 * 65535.f));
            }
            inline float decode(uint16_t u) const
            {
                return (u / 65535.f) * 2.f - 1.f;
            }
            inline float sign(float f) const
            {
                return std::copysign(1.f, f);
            }

        public:

            OctaedralVector() : x(0), y(0) {}
            OctaedralVector(const Vector<float, 3>& v)
            {   
                auto dist = std::abs(v.x) + std::abs(v.y) + std::abs(v.z);

                if (dist < 1e-6f) 
                {
                    x = encode(0.0f);
                    y = encode(1.0f); 
                    return;
                }

                float px = v.x / dist;
                float py = v.y / dist;

                if(v.z < 0.f)
                {
                    float tx = px;
                    px = (1.f - std::abs(py)) * sign(tx);
                    py = (1.f - std::abs(tx)) * sign(py);
                }   

                x = encode(px);
                y = encode(py);
      
            }
            explicit operator Vector<float, 3>() const 
            {
                float px = decode(x);
                float py = decode(y);

                Vector<float, 3> v;

                v.x = px;
                v.y = py;
                v.z =  1.f - std::abs(px) + std::abs(py);

                if (v.z < 0.f) 
                {
                  float xo = v.x;
                  v.x = (1 - std::abs(v.y)) * sign(xo);
                  v.y = (1 - std::abs(xo))  * sign(v.y);
                }

              return normalize(v);
            }
        };

};

#endif //< OCTAEDRAL_VECTOR_HPP