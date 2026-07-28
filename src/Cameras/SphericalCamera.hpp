#ifndef SPHERICAL_CAMERA_HPP
#define SPHERICAL_CAMERA_HPP

#include <Cameras/Camera.hpp>

namespace Cam 
{
    class SphericalCamera : public Camera 
    {
        public:
            enum class Mapping
            {
                EQUI_RETANGULAR=0,
                EQUAL_AREA,
            };

        private:
            Mapping map;

        public:

            SphericalCamera(std::unique_ptr<Film>& film,const Medium* medium, const Transform* cameraToWorld /*const AnimatedTransform& cameraToWorld*/, 
                            float shutterOpen, float shutterClose, Mapping map)
                            : Camera(film, medium, cameraToWorld, shutterOpen, shutterClose), map(map)
                            {};

            Ray generateRay(/*const CameraSample&*/ int x, int y) const override;
            RayDifferential generateRayDifferential(/*const CameraSample&*/int x, int y) const override;
            

    };

}; //< namespace Cam

#endif 