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
            Color f(const Vec3&, const Vec3&, const Normal3&) const override{return color;};
            Color sampleF(const Vec3&, const Normal3&, const Point2&,
                                   Vec3*, float*) const override {return color;}
    };

}; //< namespace Mat


#endif //< FLAT_MATERIAL_HPP