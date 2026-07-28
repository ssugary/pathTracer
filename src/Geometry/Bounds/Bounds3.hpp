#ifndef BOUNDS3_HPP
#define BOUNDS3_HPP

#include "ssmath3/ssmath3.hpp"


using namespace ssmath3;

namespace Geo 
{
    struct Ray;

    template<typename T>
    class Bounds3
    {   
        public:

            Point<T, 3> pMin{};
            Point<T, 3> pMax{};

            Bounds3();

            Bounds3(const Point<T, 3> &p);
            Bounds3(const Point<T, 3> &p1, const Point<T, 3> &p2);

            const Point<T, 3> &operator[](std::size_t i) const;
            Point<T, 3> &operator[](std::size_t i);
            Point<T, 3> corner(int corner) const;

            Vector<T, 3> diagonal() const ;
            float area() const ;
            float volume() const ;

            std::size_t maximumExtent() const ;
            Point<T, 3> lerp(const Point<float, 3>& p) const;
            Vector<T, 3> offset(const Point<T, 3> &p) const;

            void boundingSphere(Point<T, 3>* center, float* radius) const ;
            bool intersectP(const Ray& ray, float* hit1, float* hit2) const;
            bool intersectP(const Ray& ray, const Vector<T, 3>& invDir, const int dirIsNeg[3]) const;
    };

    
    template<typename T>
    inline Bounds3<T> boundUnion(const Bounds3<T> &b, const Point<T, 3> &p) 
    {
        return Bounds3<T>(Point<T, 3>( std::min(b.pMin.x, p.x),
                                       std::min(b.pMin.y, p.y),
                                       std::min(b.pMin.z, p.z)),

                          Point<T, 3>( std::max(b.pMax.x, p.x),
                                       std::max(b.pMax.y, p.y),
                                       std::max(b.pMax.z, p.z)));
    }
    template<typename T>
    inline Bounds3<T> boundUnion(const Bounds3<T> &b1, const Bounds3<T> &b2) 
    {
        return Bounds3<T>(Point<T, 3>( std::min(b1.pMin.x, b2.pMin.x),
                                       std::min(b1.pMin.y, b2.pMin.y),
                                       std::min(b1.pMin.z, b2.pMin.z)),

                        Point<T, 3>( std::max(b1.pMax.x, b2.pMax.x),
                                     std::max(b1.pMax.y, b2.pMax.y),
                                     std::max(b1.pMax.z, b2.pMax.z)));
    }

    template<typename T>
    inline Bounds3<T> intersection(const Bounds3<T> &b1, const Bounds3<T> &b2) 
    {
        return Bounds3<T>(Point<T, 3>( std::max(b1.pMin.x, b2.pMin.x),
                                       std::max(b1.pMin.y, b2.pMin.y),
                                       std::max(b1.pMin.z, b2.pMin.z)),
                                
                          Point<T, 3>( std::min(b1.pMax.x, b2.pMax.x),
                                       std::min(b1.pMax.y, b2.pMax.y),
                                       std::min(b1.pMax.z, b2.pMax.z)));
    }
    
    template<typename T>
    inline bool overlaps(const Bounds3<T> &b1, const Bounds3<T> &b2) 
    {
        bool x = (b1.pMax.x >= b2.pMin.x) && (b1.pMin.x <= b2.pMax.x);
        bool y = (b1.pMax.y >= b2.pMin.y) && (b1.pMin.y <= b2.pMax.y);
        bool z = (b1.pMax.z >= b2.pMin.z) && (b1.pMin.z <= b2.pMax.z);

        return (x && y && z);
    }
    template<typename T>
    inline bool inside(const Point<T, 3> &p, const Bounds3<T> &b) 
    {
        return (p.x >= b.pMin.x && p.x <= b.pMax.x &&
                p.y >= b.pMin.y && p.y <= b.pMax.y &&
                p.z >= b.pMin.z && p.z <= b.pMax.z);
    }
    template<typename T>
    inline bool insideExclusive(const Point<T, 3> &p, const Bounds3<T> &b) 
    {
        return (p.x >= b.pMin.x && p.x < b.pMax.x &&
                p.y >= b.pMin.y && p.y < b.pMax.y &&
                p.z >= b.pMin.z && p.z < b.pMax.z);
    }
    template<typename T>
    inline Bounds3<T> expand(const Bounds3<T> &b, float delta)
    {
        return Bounds3<T>( b.pMin - Vector<T, 3>(delta, delta, delta),
                         b.pMax + Vector<T, 3>(delta, delta, delta));
    }

}

#endif