#include "RelativisticCamera.hpp"
#include <Geometry/Rays/GeodesicRay.hpp>

namespace Cam 
{
      RelativisticCamera::RelativisticCamera(const Transform* CameraToWorld,
                       const Transform& CameraToScreen,
                       const Bounds2f& screenWindow,
                       std::unique_ptr<Film>& film,
                       const Medium* medium,
                       float shutterOpen,
                       float shutterClose,
                       float lensR, 
                       float focalD,
                       const Point4& pos4D, 
                       const Transform& tetrad)
          : ProjectiveCamera(CameraToWorld, CameraToScreen, screenWindow, film, 
                             medium, shutterOpen, shutterClose, lensR, focalD),
            pos(pos4D), tetrad(tetrad) {}

 
        Ray RelativisticCamera::generateRay(int, int) const 
        {
          return Ray();
        }
        RayDifferential RelativisticCamera::generateRayDifferential(int, int) const 
        {
          return RayDifferential();
        }

        GeodesicRay RelativisticCamera::generateGeodesicRay(int x, int y) const 
        {
          Vec3 pRaster(x + 0.5f, y + 0.5f, 0.f);
          Vec3 pCamera = R2C(pRaster);

          Vec3 d = ::normalize(static_cast<Vec3>(pCamera));

          Vec4 k = Vec4(1.f, -d.x, -d.y, -d.z);
          
          return GeodesicRay(pos, tetrad(k));

        }

} // namespace Cam
