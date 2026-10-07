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
        Point3 p(x + 0.5f, y + 0.5f, 0.f);
        Point3 camPos = R2C(p);
        RayDifferential ray(camPos, Vec3(0.f, 0.f, -1.f));
        /*TODO: modify ray for depths of field*/
        if(lensR > 0)
        {
            float ft = focalD / ray.d.z;
            Point3 focus = (camPos + dx) + (ft * Vec3(0.f, 0.f, -1.f));
            ray.rxOrigin = camPos + dx;
            ray.rxDirection = normalize(focus - ray.rxOrigin); 
            
            focus = camPos + dy + (ft * Vec3(0.f, 0.f, -1.f));
            ray.ryOrigin = camPos + dy;
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