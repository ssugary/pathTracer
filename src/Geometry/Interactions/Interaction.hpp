#pragma once

#ifndef INTERACTION_HPP
#define INTERACTION_HPP

#include "Geometry/Rays/Ray.hpp"

namespace Geo{

    class Interaction 
    {
        public:
            Point3 p;
            float time;
            Vec3 pError;
            Vec3 wo;
            Normal3 n;
            MediumInterface mediumInterface;
            Interaction(const Interaction&) noexcept = default;
            Interaction() : p(), time(), pError(), wo(), n(), mediumInterface() {};
            Interaction(const Point3 &p, const Normal3 &n, const Vec3 &pError,
                        const Vec3 &wo, float time,
                        const MediumInterface &mediumInterface)
                    : p(p), time(time), pError(pError), wo(wo), n(n), mediumInterface(mediumInterface) {}
                
            bool isSurfaceInteraction() const 
            {
                return n != Normal3();
            }
            Ray spawnRay(const Vec3 &d) const 
            {
                Point3 origin = isSurfaceInteraction() ? offsetRayOrigin(p, pError, n, d) : p;
                
                return Ray(origin, d, SHADOW_EPSILON, INF, time);
            }
            Ray spawnRayTo(const Point3 &p2) const {

                float realDist = distance(p2, p);

                Vec3 dir = (realDist > 0.f) ? ((p2 - p) / realDist) : (Vec3(0.f));

                Point3 origin = isSurfaceInteraction() ? offsetRayOrigin(p, pError, n, dir) : p;
                float tMax = (realDist > SHADOW_EPSILON) ? (realDist - SHADOW_EPSILON) : 0.f;
                return Ray(origin, dir, SHADOW_EPSILON, tMax, time);
            }

            Ray spawnRayTo(const Interaction &it) const 
            {
                return spawnRayTo(it.p);
            }
            

            // bool IsMediumInteraction() const { return !IsSurfaceInteraction(); }
            // const Medium *GetMedium(const Vec3 &w) const {
            //     return dot(w, n) > 0 ? mediumInterface.outside :
            //                             mediumInterface.inside;
            // }
            // const Medium *GetMedium() const {
            //     if(mediumInterface.inside == mediumInterface.outside)
            //         return mediumInterface.inside;
            //     else  
            //         return nullptr;
            // }
    };
}

#endif