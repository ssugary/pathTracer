#pragma once
#ifndef RELATIVISTIC_CAMERA_HPP
#define RELATIVISTIC_CAMERA_HPP

#include <Cameras/ProjectiveCamera.hpp>
#include "Geometry/Rays/GeodesicRay.hpp"


namespace Cam  
{

    class RelativisticCamera : public ProjectiveCamera
    {
        protected:

          Point4 pos;
          Transform tetrad;

        public:
          RelativisticCamera(const Transform* CameraToWorld,
                             const Transform& CameraToScreen,
                             const Bounds2f& screenWindow,
                             std::unique_ptr<Film>& film,
                             const Medium* medium,
                             float shutterOpen,
                             float shutterClose,
                             float lensR, 
                             float focalD,
                             const Point4& pos4D, 
                             const Transform& tetrad);

 
        Ray generateRay(int x, int y) const override;
        RayDifferential generateRayDifferential(int x, int y) const override;
        virtual GeodesicRay generateGeodesicRay(int x, int y) const;
        
            
    };
    
} // namespace Cam 


#endif // RELATIVISTIC_CAMERA_HPP 
