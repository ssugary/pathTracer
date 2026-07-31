#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "Utils/common.hpp"

namespace Mat 
{

    class Material 
    {
        protected:
            Color mirror;
        public:
            Material(const Color& mirror) : mirror(mirror) {};
            virtual ~Material() = default;
            virtual Color kd() const = 0;
            virtual Color km() const = 0;    

            virtual Color f(const Vec3& wo, const Vec3& wi, const Normal3& n) const = 0;
            virtual Color sampleF(const Vec3& wo, const Normal3& n, const Point2& u,
                                   Vec3* wi, float* pdf) const = 0;
    };

}; //< namespace Mat


#endif //< MATERIAL_HPP