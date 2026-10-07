#include "Texture.hpp"

namespace Geo 
{

    template<typename T>
    ConstantTexture<T>::ConstantTexture(const T& value)
    : value(value) {};

    template<typename T>
    T ConstantTexture<T>::evaluate(const SurfaceInteraction&) const
    {
        return value;
    }
    template<typename T1, typename T2>
    ScaleTexture<T1, T2>::ScaleTexture(const std::shared_ptr<Texture<T1>>& t1, const std::shared_ptr<Texture<T2>>& t2)
    : t1(t1), t2(t2) {};


    template<typename T1, typename T2>
    T2 ScaleTexture<T1, T2>::evaluate(const SurfaceInteraction& sf) const 
    {
        return t1->evaluate(sf) * t2->evaluate(sf);
    }

    template<typename T>
    MixedTexture<T>::MixedTexture(const std::shared_ptr<Texture<T>>& t1, const std::shared_ptr<Texture<T>>& t2, const std::shared_ptr<Texture<float>>& amount)
    : t1(t1), t2(t2), amount(amount) {};

    template<typename T>
    T MixedTexture<T>::evaluate(const SurfaceInteraction& sf) const 
    {
        float amt = amount->evaluate(sf);
        return (1 - amt) * t1->evaluate(sf) + amt * t2->evaluate(sf);
    }

    template<typename T>
    BilerpTexture<T>::BilerpTexture(std::unique_ptr<TextureMapping2D> mapping, const T& br, const T& tr, const T& tl, const T& bl)
    : mapping(std::move(mapping)), br(br), tr(tr), tl(tl), bl(bl) {};

    template<typename T>
    T BilerpTexture<T>::evaluate(const SurfaceInteraction& sf) const 
    {

        Point2 st = mapping->map(sf, nullptr, nullptr);
        
        const T bH = (1 - st.x) * bl +  st.x * br;
        const T tH = (1 - st.x) * tl +  st.x * tr;

        return (1 - st.y) * bH + st.y * tH;
    }

    template<typename T>
    ImageTexture<T>::ImageTexture(std::unique_ptr<TextureMapping2D> mapping, std::shared_ptr<MIPMap<T>> mipmap)
    : mapping(std::move(mapping)), mipmap(std::move(mipmap)) {};

    template<typename T>
    T ImageTexture<T>::evaluate(const SurfaceInteraction& sf) const 
    {
        Vec2 dstdx;
        Vec2 dstdy;

        Point2 st = mapping->map(sf, &dstdx, &dstdy);

        return mipmap->lookup(st, dstdx, dstdy);
    }

    Normal3 applyNormalMap(const SurfaceInteraction& si,
                                   const std::shared_ptr<Geo::Texture<Color>>& normalMap)
    {
        if (!normalMap)
            return si.shading.n; 

        Color s = normalMap->evaluate(si);
        Vec3 nLocal = normalize(Vec3(2.f*s.r - 1.f, 2.f * s.g - 1.f, 2.f * s.b - 1.f));

        Vec3 T = normalize(si.shading.dpdu);                
        Vec3 B = cross(static_cast<Vec3>(si.shading.n), T);

        Vec3 nsWorld = nLocal.x * T + nLocal.y * B + nLocal.z * static_cast<Vec3>(si.shading.n);
        Normal3 result = normalize(Normal3(nsWorld));

        return faceFoward(result, si.n);
    }


    template class Texture<float>;
    template class Texture<Color>;

    template class ImageTexture<float>;
    template class ImageTexture<Color>;

    template class ConstantTexture<float>;
    template class ConstantTexture<Color>;

    template class BilerpTexture<float>;
    template class BilerpTexture<Color>;


};
