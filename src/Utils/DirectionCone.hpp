#ifndef DIRECTION_CONE_HPP
#define DIRECTION_CONE_HPP

#include <Geometry/Transformation/Transform.hpp>
#include <Utils/common.hpp>
#include <ssmath3/ssmath3.hpp>
namespace ssrt 
{

    class DirectionCone 
    {
        public:

            Vec3 w;
            float cosTheta = INF;

            DirectionCone() = default;
            DirectionCone(const Vec3& w, const float cosTheta=1)
            : w(::normalize(w)), cosTheta(cosTheta) {}

            static DirectionCone entireSphere()
            {
                return DirectionCone(Vec3(0.f, 0.f, 1.f), -1);
            }
            inline bool isEmpty() const
            {
                return cosTheta == INF;
            }
    };

    inline bool inside(const DirectionCone& d, const Vec3& w)  
    {
        return !d.isEmpty() && dot(d.w, normalize(w)) >= d.cosTheta;
    }

    inline DirectionCone boundSubtendedDirections(const Bounds3f& b, const Point3& p)
    {
        float radius;
        Point3 center;

        b.boundingSphere(&center, &radius);
        if(sqrDist(p, center) < radius * radius)
            return DirectionCone::entireSphere();

        Vec3 w = normalize(center - p);
        float sin2Theta = radius * radius / sqrDist(p, center);
        return DirectionCone(w, std::sqrt(std::max(0.f, 1 - sin2Theta)));
    }
    inline DirectionCone coneUnion(const DirectionCone& da, const DirectionCone& db)
    {
        if(da.isEmpty())
            return db;
        if(db.isEmpty())
            return da;

        float thetaA = std::acos(da.cosTheta);
        float thetaB = std::acos(db.cosTheta);
        float thetaD = angle(da.w, db.w);

        if(std::min(thetaD + thetaB, PI) <= thetaA)
            return da;
        if(std::min(thetaD + thetaA, PI) <= thetaB)
            return db;

        float thetaO = (thetaA + thetaB + thetaD)/2.f;
        if(thetaO >= PI)
            return DirectionCone::entireSphere();

        Vec3 wr = cross(da.w, db.w); 
        if(sqrLength(wr) == 0)
            return DirectionCone::entireSphere();

        float thetaR = thetaO - thetaA; // theta real finalmente...
        Vec3 w = Transform::rotate(thetaR, static_cast<Point3>(wr))(da.w);
        return DirectionCone(w, std::cos(thetaO));
    }


}; //< namespace ssrt


#endif //< DIRECTION_CONE_HPP