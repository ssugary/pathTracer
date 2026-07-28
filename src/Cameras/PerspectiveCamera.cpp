#include "PerspectiveCamera.hpp"


namespace Cam 
{

    Ray PerspectiveCamera::generateRay(/*const CameraSample&*/ int x, int y) const 
    {
        Point3 p(x + 0.5f, y + 0.5f, 0.f);
        Point3 pCam = R2C(p);
        
        Ray ray(Point3(0.f), normalize(static_cast<Vec3>(pCam)));
        ray.medium = medium;
        
        return (*C2W)(ray);
    }


    RayDifferential PerspectiveCamera::generateRayDifferential(/*const CameraSample&*/int x, int y) const 
    {
        Point3 p(x + 0.5f, y + 0.5f, 0.f);
        Point3 camPos = R2C(p);
        RayDifferential ray(Point3(0, 0, 0), normalize(static_cast<Vec3>(camPos)));
        
        /*TODO offsets of Perspective Camera*/
        // ray.time = ::lerp(cs.time, shutterOpen, shutterClose);
        ray.medium = medium;

        return (*C2W)(ray);
    }   
};