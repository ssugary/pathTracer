#ifndef CAMERA_HPP
#define CAMERA_HPP


#include "Geometry/Rays/Ray.hpp"
#include "Geometry/Rays/RayDifferential.hpp"
#include "Film.hpp"
#include "Geometry/Transformation/AnimatedTransform.hpp"
#include <memory>

using namespace Geo;

namespace Cam 
{

    class Camera 
    {
        protected:
          
            const Medium* medium;
            //AnimatedTransform cameraToWorld;
            const Transform* C2W;
            const float shutterOpen;
            const float shutterClose;
        
        public:

            Camera(std::unique_ptr<Film>& film,const Medium* medium, const Transform* cameraToWorld /*const AnimatedTransform& cameraToWorld*/, float shutterOpen, float shutterClose)
            : medium(std::move(medium)), C2W(std::move(cameraToWorld)), shutterOpen(shutterOpen), shutterClose(shutterClose), film(std::move(film)) {};
            
            ~Camera() = default;
            virtual Ray generateRay(/*const CameraSample&*/ int x, int y) const = 0;
            virtual RayDifferential generateRayDifferential(/*const CameraSample&*/int x, int y) const = 0;
            
            std::unique_ptr<Film> film;
    };
};

#endif
