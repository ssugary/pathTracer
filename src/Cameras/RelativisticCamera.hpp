#pragma once
#ifndef RELATIVISTIC_CAMERA_HPP
#define RELATIVISTIC_CAMERA_HPP

#include <Cameras/ProjectiveCamera.hpp>
#include "Geometry/Rays/GeodesicRay.hpp"
#include "Geometry/Metric/Metric.hpp"


namespace Cam  
{

    class RelativisticCamera : public ProjectiveCamera
    {
        protected:

          Point4 pos;
          Transform tetrad;
          const std::shared_ptr<Geo::Metric> metric;

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
                            std::shared_ptr<Geo::Metric> metric);

 
        Ray generateRay(float x, float y) const override;
        RayDifferential generateRayDifferential(float x, float y) const override;
        virtual GeodesicRay generateGeodesicRay(float x, float y) const;
        
            
    };
    
} // namespace Cam 


#endif // RELATIVISTIC_CAMERA_HPP 
