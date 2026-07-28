#include "Bounds3.hpp"
#include "Geometry/Rays/Ray.hpp"

namespace Geo 
{

    template<typename T>
    Bounds3<T>::Bounds3() 
    {
        float minNum = std::numeric_limits<float>::lowest();
        float maxNum = std::numeric_limits<float>::max();
        pMin = Point<T, 3>(maxNum, maxNum, maxNum);
        pMax = Point<T, 3>(minNum, minNum, minNum);
    }

    template<typename T>
    Bounds3<T>::Bounds3(const Point<T, 3> &p) : pMin(p), pMax(p) {}

    template<typename T>
    Bounds3<T>::Bounds3(const Point<T, 3> &p1, const Point<T, 3> &p2)
    :   pMin(std::min(p1.x, p2.x), std::min(p1.y, p2.y),
        std::min(p1.z, p2.z)),
        pMax(std::max(p1.x, p2.x), std::max(p1.y, p2.y),
        std::max(p1.z, p2.z)) 
    {}
    template<typename T>
    const Point<T, 3> &Bounds3<T>::operator[](std::size_t i) const
    {
        assert(i < 2);

        return i == 0 ? pMin : pMax;
    }
    template<typename T>
    Point<T, 3> &Bounds3<T>::operator[](std::size_t i)
    {
        assert(i < 2);

        return i == 0 ? pMin : pMax;
    }
    
    template<typename T>
    Point<T, 3> Bounds3<T>::corner(int corner) const 
    {
        return Point<T, 3>((*this)[(corner & 1)].x,
                            (*this)[(corner & 2) ? 1 : 0].y,
                            (*this)[(corner & 4) ? 1 : 0].z
                            );
    }

    template<typename T>
    Vector<T, 3> Bounds3<T>::diagonal() const 
    { 
        return pMax - pMin; 
    }

    template<typename T>
    float Bounds3<T>::area() const 
    {
        Vector<T, 3> d = diagonal();
        return 2 * (d.x * d.y + d.x * d.z + d.y * d.z);
    }
    template<typename T>
    float Bounds3<T>::volume() const 
    {
        Vector<T, 3> d = diagonal();
        return d.x * d.y * d.z;
    }

    template<typename T>
    std::size_t Bounds3<T>::maximumExtent() const 
    {
        Vector<T, 3> d = diagonal();

        if (d.x > d.y && d.x > d.z)
            return 0;
        else if (d.y > d.z)
            return 1;
        else
            return 2;
    }

    template<typename T>
    Point<T, 3> Bounds3<T>::lerp(const Point<float, 3>& p) const
    {
        return Point<T, 3>{static_cast<T>(::lerp(p.x, pMin.x, pMax.x)), static_cast<T>(::lerp(p.y, pMin.y, pMax.y)), static_cast<T>(::lerp(p.z, pMin.z, pMax.z))};
    }
        
    template<typename T>
    Vector<T, 3> Bounds3<T>::offset(const Point<T, 3> &p) const 
    {
        Vector<T, 3> o = p - pMin;

        if (pMax.x > pMin.x) 
            o[0] /= pMax.x - pMin.x;
        if (pMax.y > pMin.y) 
            o[1] /= pMax.y - pMin.y;
        if (pMax.z > pMin.z) 
            o[2] /= pMax.z - pMin.z;

        return o;
    }

    template<typename T>
    void Bounds3<T>::boundingSphere(Point<T, 3>* center, float* radius) const 
    {
        *center = (pMin + pMax)/2;
        *radius = inside(*center, *this) ? distance(*center, pMax) : 0;
    }

    template<typename T>
    bool Bounds3<T>::intersectP(const Ray& ray, float* hit1, float* hit2) const
    {
        float t0 = ray.tMin;
        float t1 = ray.tMax;

        for(int i{0}; i < 3; ++i)
        {
            if (ray.d[i] == 0.0f)
            {
                if (ray.o[i] < pMin[i] || ray.o[i] > pMax[i])
                    return false;
                continue; 
            }

            float invRayDir = 1 / ray.d[i];
            float tNear = (pMin[i] - ray.o[i]) * invRayDir;
            float tFar = (pMax[i] - ray.o[i]) * invRayDir;
            
            if(tNear > tFar)
                std::swap(tNear, tFar);

            tFar *= 1 + 2 * gamma(3);

            if(tNear > t0)
                t0 = tNear;
            if(tFar < t1)
                t1 = tFar;

            if (t0 > t1) 
                return false;
        }

        if (hit1) 
            *hit1 = t0;
        if (hit2) 
            *hit2 = t1;

        return true;
    }

    template<typename T>
    bool Bounds3<T>::intersectP(const Ray& ray, const Vector<T, 3>& invDir, const int dirIsNeg[3]) const
    {
        float tMin =  ((*this)[  dirIsNeg[0]].x - ray.o.x) * invDir.x;
        float tMax =  ((*this)[1-dirIsNeg[0]].x - ray.o.x) * invDir.x;
        float tyMin = ((*this)[  dirIsNeg[1]].y - ray.o.y) * invDir.y;
        float tyMax = ((*this)[1-dirIsNeg[1]].y - ray.o.y) * invDir.y;

        if (tMin > tyMax || tyMin > tMax) 
            return false;
        
        if (tyMin > tMin) 
            tMin = tyMin;
        if (tyMax < tMax) 
            tMax = tyMax;

        float tzMin = ((*this)[  dirIsNeg[2]].z - ray.o.z) * invDir.z;
        float tzMax = ((*this)[1-dirIsNeg[2]].z - ray.o.z) * invDir.z;

        if (tMin > tzMax || tzMin > tMax) 
            return false;
        
        if (tzMin > tMin) 
            tMin = tzMin;
        
        if (tzMax < tMax) 
            tMax = tzMax;

        return (tMin < ray.tMax) && (tMax > ray.tMin);
    }


    template class Bounds3<float>;
    template class Bounds3<int>;

    

};