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

    };

}; //< namespace Mat


#endif //< MATERIAL_HPP