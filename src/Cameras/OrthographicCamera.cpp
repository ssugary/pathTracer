#include "OrthographicCamera.hpp"
#include "Utils/common.hpp"


namespace Cam 
{

    Ray OrthographicCamera::generateRay(/*const CameraSample&*/ int x, int y) const 
    {
        Point3 p(x + 0.5f, y + 0.5f, 0.f);
        Point3 pCam = R2C(p);
        Ray ray(pCam, Vec3(0.f, 0.f, -1.f));
        // /*TODO: modify ray for depths of field*/
        // ray.time = lerp(cs.time, shutterOpen, shutterClose);
        ray.medium = medium;
        return (*C2W)(ray);


    }
    RayDifferential OrthographicCamera::generateRayDifferential(/*const CameraSample&*/int x, int y) const 
    {
        Point3 p(x, y, 0);
        Point3 camPos = R2C(p);
        RayDifferential ray(camPos, Vec3(0, 0, 1));
        /*TODO: modify ray for depths of field*/
        if(lensR > 0)
        {
            float ft = focalD / ray.d[2];
            Point3 focus = (camPos + dx) + (ft * Vec3(0, 0, 1));
            ray.rxOrigin = Point3(p[0], p[1], 0);
            ray.rxDirection = normalize(focus - ray.rxOrigin); 
            
            focus = camPos + dy + (ft * Vec3(0, 0, 1));
            ray.ryOrigin = Point3(p[0], p[1], 0);
            ray.ryDirection = normalize(focus - ray.ryOrigin);
        }
        else 
        {
            // ray.time = lerp(cs.time, shutterOpen, shutterClose);
            ray.rxOrigin = ray.o + dx;
            ray.ryOrigin = ray.o + dy;
            ray.rxDirection = ray.d;
            ray.ryDirection = ray.d;
        }

        ray.hasDifferentials = true;
        ray.medium = medium;
        
        return (*C2W)(ray);
        
    }

}   //< namespace Cam