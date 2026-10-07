#pragma once

#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "TextureMap.hpp"
#include "MipMap.hpp"

namespace Geo 
{
    template<typename T>
    class Texture 
    {
        public:

            virtual ~Texture() = default;
            virtual T evaluate(const SurfaceInteraction& sf) const = 0;
    };

    template<typename T>
    class ConstantTexture : public Texture<T>
    {
        private:
            T value;
        public:
            ConstantTexture(const T& value);
            T evaluate(const SurfaceInteraction& sf) const override;
    };
    template<typename T1, typename T2>
    class ScaleTexture : public Texture<T2>
    {
        private:
            std::shared_ptr<Texture<T1>> t1;
            std::shared_ptr<Texture<T2>> t2;
        public:
            ScaleTexture(const std::shared_ptr<Texture<T1>>& t1, const std::shared_ptr<Texture<T2>>& t2);
            T2 evaluate(const SurfaceInteraction& sf) const override;
    };
    
    template<typename T>
    class MixedTexture : public Texture<T>
    {
        private:
            std::shared_ptr<Texture<T>> t1;
            std::shared_ptr<Texture<T>> t2;
            std::shared_ptr<Texture<float>> amount;
        public:
            MixedTexture(const std::shared_ptr<Texture<T>>& t1, const std::shared_ptr<Texture<T>>& t2, const std::shared_ptr<Texture<float>>& amount);
            T evaluate(const SurfaceInteraction& sf) const override;
    };

    template<typename T>
    class BilerpTexture : public Texture<T>
    {
        private:
            std::unique_ptr<TextureMapping2D> mapping;
            T br;
            T tr;
            T tl;
            T bl;
        public:
            BilerpTexture(std::unique_ptr<TextureMapping2D> mapping, const T& br, const T& tr, const T& tl, const T& bl);
            T evaluate(const SurfaceInteraction& sf) const override;
    };

    template<typename T>
    class ImageTexture : public Texture<T>
    {
        private:
            std::unique_ptr<TextureMapping2D> mapping;
            std::shared_ptr<MIPMap<T>> mipmap;
        public:
            ImageTexture(std::unique_ptr<TextureMapping2D> mapping, std::shared_ptr<MIPMap<T>> mipmap);
            T evaluate(const SurfaceInteraction& sf) const override;
    };

    Normal3 applyNormalMap(const SurfaceInteraction& si, const std::shared_ptr<Geo::Texture<Color>>& normalMap);
}


#endif //< TEXTURE_HPP