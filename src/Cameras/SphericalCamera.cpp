#include "SphericalCamera.hpp"
#include <Utils/common.hpp>

namespace Cam 
{

    Ray SphericalCamera::generateRay(/*const CameraSample&*/ int x, int y) const
    {
        Point2 uv{x/static_cast<float>(film->fullRes[0]), y/static_cast<float>(film->fullRes[1])};
        Vec3 dir;
        switch(map)
        {
            case Mapping::EQUI_RETANGULAR:
            {
                float theta = PI * uv.y;
                float phi = 2 * PI * uv.x;
                dir = sphericalDirection(std::sin(theta), std::cos(theta), phi);
                break;
            }
            default : //< Mapping::EQUAL_AREA
            {
                uv = wrapSquare(uv);
                dir = squareToSphere(uv);
                break;
            }
        }
        std::swap(dir.y, dir.z);
        return (*C2W)(Ray(Point3(0.f), dir));
    }
    RayDifferential SphericalCamera::generateRayDifferential(/*const CameraSample&*/int, int) const
    {
    // Point2 uv{x/static_cast<float>(film->fullRes[0]), y/static_cast<float>(film->fullRes[1])};
    // Vec3 dir;
    // switch(map)
    // {
    //     case Mapping::EQUI_RETANGULAR:
    //     {
    //         float theta = PI * uv.y;
    //         float phi = 2 * PI * uv.x;
    //         dir = sphericalDirection(std::sin(theta), std::cos(theta), phi);
    //         break;
    //     }
    //     default : //< Mapping::EQUAL_AREA
    //     {
    //         uv = wrapSquare(uv);
    //         dir = SquareToSphere(uv);
    //     }
    //     std::swap(dir.y, dir.z);
    // }
    // return (*C2W)(Ray(Point3(0.f), dir));
        return {};
    }

};