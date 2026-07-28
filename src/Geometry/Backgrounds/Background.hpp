#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include "Utils/common.hpp"

namespace Prim 
{
    class Background 
    {
        public:

            virtual ~Background() = default;
            virtual Color sample(const Vec3& d, float=1.f, float=1.f) const = 0;
    };
};

#endif //< BACKGROUND_HPP