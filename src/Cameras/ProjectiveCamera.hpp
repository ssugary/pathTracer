#ifndef PROJECTIVE_CAMERA_HPP
#define PROJECTIVE_CAMERA_HPP 

#include "Camera.hpp"

namespace Cam 
{
    class ProjectiveCamera : public Camera
    {
        protected:
            Transform C2S;
            Transform R2C;

            Transform S2R;
            Transform R2S;
                
            float lensR;  //< Lens Radius
            float focalD; //< Focal Distance
        public:
            

            ProjectiveCamera(const Transform* cameraToWorld /*const AnimatedTransform& cameraToWorld*/, const Transform& cameraToScreen, 
                             const Bounds2f& screenWindow,
                             std::unique_ptr<Film>& film, const Medium* medium,
                             float shutterOpen, float shutterClose,
                             float lensR, float focalD)

                            : Camera(film, medium, cameraToWorld, shutterOpen, shutterClose),
                             C2S(cameraToScreen), lensR(lensR), focalD(focalD)
            {
                Point2i res = this->film->fullRes;
                S2R = Transform::scale(res[0], res[1], 1) 
                        (
                        Transform::scale(1 / (screenWindow.pMax.x - screenWindow.pMin.x),
                                            1 / (screenWindow.pMin.y - screenWindow.pMax.y), 
                                            1)(
                                                Transform::translate(Point3(-screenWindow.pMin.x, -screenWindow.pMax.y, 0))));
                R2S = Transform::inverse(S2R);
                R2C = Transform::inverse(cameraToScreen)(R2S);

            }
        };
} //< namespace Cam


#endif //< PROJECTIVE_CAMERA_HPP