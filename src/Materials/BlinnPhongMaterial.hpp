#ifndef BLINN_PHONG_MATERIAL_HPP
#define BLINN_PHONG_MATERIAL_HPP

#include "Material.hpp"

namespace Mat{

    class BlinnPhongMaterial : public Material{
        private:
            Color diffuse;   //< color that indicates how much diffuse color is reflected.
            Color specular;  //< color that represents the color of the specular highlights.
            Color ambient;   //< color that represents how much the incoming light is reflected.
            float glossiness;  //< value that control how narrowed is the specular highlight in the scene.
        public:
        BlinnPhongMaterial(const Color& mirror = Color()) : Material(mirror), diffuse(), specular(), ambient(), glossiness() {};
        BlinnPhongMaterial(const Color& diffuse, const Color& specular, const Color& ambient, float glossiness, const Color& mirror = Color()) :
                          Material(mirror), diffuse(diffuse), specular(specular), ambient(ambient), glossiness(glossiness) {};

        Color          km() const override {return mirror;};   //< mirror's getter
        virtual Color  kd() const override {return diffuse;};  //< diffuse coeficient's getter
        virtual Color  ks() const {return specular;};          //< specular coeficient's getter
        virtual Color  ka() const {return ambient;};           //< ambient coeficient's getter
        virtual float gg() const {return glossiness;};        //< glossiness's getter 
        


    };

}
#endif