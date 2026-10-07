#include "MipMap.hpp"

namespace Geo 
{
    template<typename T>
    float MIPMap<T>::weightLut[128];

    template<typename T>
    T MIPMap<T>::texel(int level, int x, int y) const
    {
        const Array2D<T>& img = *pyramid[level];

        int w = img.width;
        int h = img.height;

        switch (wrapMode) 
        {
            case WrapMode::REPEAT:
                x = (x % w + w) % w; 
                y = (y % h + h) % h;
                break;
            case WrapMode::CLAMP:
                x = std::clamp(x, 0, w - 1);
                y = std::clamp(y, 0, h - 1);
                break;
            case WrapMode::BLACK:
                if (x < 0 || x >= w || y < 0 || y >= h) 
                    return static_cast<T>(0.f); 
                break;
        }
        return img(x, y);
    }

    template<typename T>
    MIPMap<T>::MIPMap(const Point2i& res, T* data, WrapMode mode, bool doTrilinear, float maxAnisotropy)
    : res(res), wrapMode(mode), doTrilinear(doTrilinear), maxAnisotropy(maxAnisotropy)
    {
        static bool lutInitialized{false};
        if (!lutInitialized) 
        {
            for (int i{0}; i < 128; ++i) 
                weightLut[i] = std::exp(- static_cast<float>(i) / 64.f);
            
            lutInitialized = true;
        }
        
        pyramid.push_back(std::make_unique<Array2D<T>>(res.x, res.y, data));
        int w = res.x;
        int h = res.y;

        while(w > 1 || h > 1)
        {
            int nextW = std::max(1, w/2);
            int nextH = std::max(1, h/2);

            int prevIdx = pyramid.size() - 1;
            auto next = std::make_unique<Array2D<T>>(nextW, nextH);

            for(int j{0}; j < nextH; ++j)
            
                for(int i{0}; i < nextW; ++i)
                {
                    T sum = static_cast<T>(0.f);

                    for(int k{0}; k < 4; ++k)
                    {
                        int dx = k % 2;
                        int dy = k / 2;
                        sum += texel(prevIdx, i * 2 + dx, j * 2 + dy);
                    }

                    (*next)(i, j) = sum / 4.f;
                }
            
            pyramid.push_back(std::move(next));
            w = nextW;
            h = nextH;
        }
    }
    template<typename T>
    T MIPMap<T>::EWA(int level, Point2 st, Vec2 dstdx, Vec2 dstdy) const
    {
        if (level >= (int) pyramid.size()) 
            return texel(pyramid.size() - 1, 0, 0);

        st[0] = st[0] * pyramid[level]->width - 0.5f;
        st[1] = st[1] * pyramid[level]->height - 0.5f;
        dstdx[0] *= pyramid[level]->width;
        dstdx[1] *= pyramid[level]->height;
        dstdy[0] *= pyramid[level]->width;
        dstdy[1] *= pyramid[level]->height;

        float A = dstdx[1] * dstdx[1] + dstdy[1] * dstdy[1] + 1;
        float B = -2 * (dstdx[0] * dstdx[1] + dstdy[0] * dstdy[1]);
        float C = dstdx[0] * dstdx[0] + dstdy[0] * dstdy[0] + 1;
        float F = A * C - B * B / 4.f;

        if (F <= 0.f)
            return triangle(level, st);
        
        A /= F;
        B /= F;
        C /= F;
    
        float det = -B * B + 4 * A * C;
        if (det <= 0.f)
            return triangle(level, st);

        float uSqrt = std::sqrt(det * C), vSqrt = std::sqrt(A * det);
        int s0 = std::ceil (st[0] - 2 * uSqrt / det);
        int s1 = std::floor(st[0] + 2 * uSqrt / det);
        int t0 = std::ceil (st[1] - 2 * vSqrt / det);
        int t1 = std::floor(st[1] + 2 * vSqrt / det);

        T sum(0.f);
        float sumWts{0.f};
        for (int it{t0}; it <= t1; ++it) 
        {
            float tt = it - st[1];

            for (int is{s0}; is <= s1; ++is) 
            {
                float ss = is - st[0];
                float r2 = A * ss * ss + B * ss * tt + C * tt * tt;
                if (r2 < 1) 
                {
                    int index = std::min(static_cast<int>(r2 * 128), 127);
                    float weight = weightLut[index];
                    sum += texel(level, is, it) * weight;
                    sumWts += weight;
                }

            }
        }

        if (sumWts == 0.f)
            return triangle(level, st);

        return sum / sumWts;
    }

    template<typename T>
    T MIPMap<T>::lookup(const Point2& st, float width) const
    {
        float level = pyramid.size() - 1 + std::log2(std::max(width, EPSILON));
        if (level < 0)
           return triangle(0, st);
        else if (level >= pyramid.size() - 1)
            return texel(pyramid.size() - 1, 0, 0);
        else 
        {
            int iLevel = std::floor(level);
            float delta = level - iLevel;
            return ::lerp(delta, triangle(iLevel, st), triangle(iLevel + 1, st));
        }
    }

    template<typename T>
    T MIPMap<T>::lookup(const Point2& st, const Vec2& dstdx, const Vec2& dstdy) const
    {

        if(doTrilinear)
        {
            float width = std::max(std::max(std::abs(dstdx.x),
                                            std::abs(dstdx.y)),
                                   std::max(std::abs(dstdy.x),
                                            std::abs(dstdy.y)));
            return lookup(st, 2 * width);
        }

        float majorLength;
        float minorLength;

        Vec2 dstdxTemp = dstdx;
        Vec2 dstdyTemp = dstdy;

        if (sqrLength(dstdxTemp) < sqrLength(dstdyTemp))
        {
            Vec2 temp = dstdxTemp;
            dstdxTemp = dstdyTemp;
            dstdyTemp = temp;
        }

        majorLength = length(dstdxTemp);
        minorLength = length(dstdyTemp);

        if (minorLength * maxAnisotropy < majorLength && minorLength > 0) 
        {
           float scale = majorLength / (minorLength * maxAnisotropy);
           dstdyTemp *= scale;
           minorLength *= scale;
       }
       if (minorLength == 0)
           return triangle(0, st);
        
        
        float lod = std::max(0.f, pyramid.size() - 1.f + std::log2(minorLength));
        int ilod = std::floor(lod);
        return ::lerp(lod - ilod, EWA(ilod, st, dstdxTemp, dstdyTemp),
                                    EWA(ilod + 1, st, dstdxTemp, dstdyTemp));
    }

    template <typename T>
    T MIPMap<T>::triangle(int level, const Point2 &st) const 
    {
        level = clamp(level, 0, pyramid.size() - 1);
        float s = st[0] * pyramid[level]->width - 0.5f;
        float t = st[1] * pyramid[level]->height - 0.5f;
        int s0 = std::floor(s), t0 = std::floor(t);
        float ds = s - s0, dt = t - t0;
        
        return (1 - ds) * (1 - dt) * texel(level, s0,   t0) +
               (1 - ds) * dt       * texel(level, s0,   t0+1) +
               ds       * (1 - dt) * texel(level, s0+1, t0) +
               ds       * dt       * texel(level, s0+1, t0+1);
    }

    template<typename T>
    T MIPMap<T>::bilerp(int level, const Point2& st) const
    {
        const Array2D<T>& img = *pyramid[level];
    
        float px = st.x * img.width - 0.5f;
        float py = st.y * img.height - 0.5f;
        
        int x0 = std::floor(px);
        int y0 = std::floor(py);
        float dx = px - x0;
        float dy = py - y0;
        
        T bl = texel(level, x0, y0);
        T br = texel(level, x0 + 1, y0);
        T tl = texel(level, x0, y0 + 1);
        T tr = texel(level, x0 + 1, y0 + 1);
        
        T bH = (1.f - dx) * bl + dx * br;
        T tH = (1.f - dx) * tl + dx * tr;

        return (1.f - dy) * bH + dy * tH;
    }


    template class MIPMap<float>;
    template class MIPMap<Color>;
};