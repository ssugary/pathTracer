#ifndef FLAT_MATERIAL_HPP
#define FLAT_MATERIAL_HPP

#include "Material.hpp"

namespace Mat 
{

    class FlatMaterial : public Material 
    {
        private:
            Color color;
        public:
            FlatMaterial(const Color& color, const Color& mirror) : Material(mirror), color(color) {};
            Color kd() const override {return color;};
            Color km() const override {return mirror;};    

    };

}; //< namespace Mat


#endif //< FLAT_MATERIAL_HPP