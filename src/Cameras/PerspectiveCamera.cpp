#include "PerspectiveCamera.hpp"


namespace Cam 
{

    Ray PerspectiveCamera::generateRay(/*const CameraSample&*/ float x, float y) const 
    {
        Point3 p(x + 0.5f, y + 0.5f, 0.f);
        Point3 pCam = R2C(p);
        
        Ray ray(Point3(0.f), normalize(static_cast<Vec3>(pCam)));
        ray.medium = medium;
        
        return (*C2W)(ray);
    }


    RayDifferential PerspectiveCamera::generateRayDifferential(/*const CameraSample&*/float x, float y) const 
    {
        Point3 p(x + 0.5f, y + 0.5f, 0.f);
        Point3 camPos = R2C(p);
        RayDifferential ray(Point3(0, 0, 0), normalize(static_cast<Vec3>(camPos)));
        
        /*TODO offsets of Perspective Camera*/
        if(lensR > 0)
        {
            Vec3 dxr = ::normalize(static_cast<Vec3>(camPos + dx));
            Vec3 dyr = ::normalize(static_cast<Vec3>(camPos + dy));
            float ft = focalD / dxr.z;
            Point3 focus = Point3(0.f) + (ft * dxr);
            ray.rxOrigin = ray.o;
            ray.rxDirection = normalize(focus - ray.rxOrigin); 
            
            ft = focalD / dyr.z;
            focus = Point3(0.f) + (ft * dyr);
            ray.ryOrigin = ray.o;
            ray.ryDirection = normalize(focus - ray.ryOrigin);
        }
        else 
        {
            // ray.time = lerp(cs.time, shutterOpen, shutterClose);
            ray.rxOrigin = ray.o;
            ray.ryOrigin = ray.o;
            ray.rxDirection = normalize(static_cast<Vec3>(camPos + dx));
            ray.ryDirection = normalize(static_cast<Vec3>(camPos + dy));
        }

        ray.medium = medium;
        ray.hasDifferentials = true;

        return (*C2W)(ray);
    }   
};