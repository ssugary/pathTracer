#pragma once

#ifndef PERSPECTIVE_CAMERA_HPP
#define PERSPECTIVE_CAMERA_HPP

#include "ProjectiveCamera.hpp"

namespace Cam 
 {

 class PerspectiveCamera : public ProjectiveCamera
   {
      private:

        Vec3 dx;
        Vec3 dy;
        
      public:
        PerspectiveCamera(const Transform* cameraToWorld /*const AnimatedTransform& cameraToWorld*/, const Transform& cameraToScreen, 
                            const Bounds2f& screenWindow,
                            std::unique_ptr<Film>& film,const Medium* medium,
                            float shutterOpen, float shutterClose,
                            float lensR, float focalD  /*fov*/
                            )
          : ProjectiveCamera(cameraToWorld, cameraToScreen, screenWindow,
                            film, medium, shutterOpen, shutterClose, 
                            lensR, focalD)
                              {
                                dx = R2C(Point3(1, 0, 0)) - R2C(Point3(0, 0, 0));
                                dy = R2C(Point3(0, 1, 0)) - R2C(Point3(0, 0, 0));
                              };
          
        virtual Ray generateRay(/*const CameraSample&*/ float x, float y) const override;
        virtual RayDifferential generateRayDifferential(/*const CameraSample&*/float x, float y) const override;

   };

 } // namespace Cam

#endif // PERSPECTIVE_CAMERA_HPP
