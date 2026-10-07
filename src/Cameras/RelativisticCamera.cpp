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
                      std::shared_ptr<Geo::Metric> metric)
          : ProjectiveCamera(CameraToWorld, CameraToScreen, screenWindow, film, 
                             medium, shutterOpen, shutterClose, lensR, focalD),
            pos(pos4D), metric(std::move(metric)) 
            {
                Mat4 g = this->metric->g(pos);

                auto metricDot = [&](const Vec4& u, const Vec4& v) -> double
                {
                  double u_arr[4] = {u.t, u.r, u.theta, u.phi};
                  double v_arr[4] = {v.t, v.r, v.theta, v.phi};
                  double sum = 0.;

                  for (int i{0}; i < 4; ++i) 
                    for (int j{0}; j < 4; ++j) 
                      sum += static_cast<double>(g[i][j]) * u_arr[i] * v_arr[j];
                    
                  
                  return sum;
                };

                Vec4 e[4];
                double gtt = g[0][0];
                double gtp = g[0][3];
                double gpp = g[3][3];

                double Omega = (std::abs(gpp) > 1e-12) ? (-gtp / gpp) : 0.0;
                double normSq = gtt + 2.0 * Omega * gtp + Omega * Omega * gpp;
                double ut = 1.0 / std::sqrt(std::max(-normSq, 1e-12));
                double uphi = Omega * ut;

                e[0] = Vec4(static_cast<float>(ut), 0.f, 0.f, static_cast<float>(uphi));

                Vec4 v[4];
                v[1] = Vec4(0.f, 1.f, 0.f, 0.f);              
                v[2] = Vec4(0.f, 0.f, 1.f, 0.f); 
                v[3] = Vec4(0.f, 0.f, 0.f, 1.f); 
                

                for (int k{1}; k <= 3; ++k) 
                {
                  Vec4 w = v[k];
                  double proj0 = metricDot(v[k], e[0]);
                  w = w + static_cast<float>(proj0) * e[0];

                  
                  for (int j{1}; j < k; ++j) 
                    w = w - static_cast<float>(metricDot(w, e[j])) * e[j];
                  

                  e[k] = w * (1.f / static_cast<float>(std::sqrt(std::max(metricDot(w, w), 1e-12))));
                }

                Mat4 M(0.f);
                for (int j{0}; j < 4; ++j) 
                {
                  float e_arr[4] = {e[j].t, e[j].r, e[j].theta, e[j].phi};
                  for (int i{0}; i < 4; ++i) 
                    M[i][j] = e_arr[i];
                  
                }

                tetrad = Geo::Transform(M);
            }

 
        Ray RelativisticCamera::generateRay(float, float) const 
        {
          return Ray();
        }
        RayDifferential RelativisticCamera::generateRayDifferential(float, float) const 
        {
          return RayDifferential();
        }

        GeodesicRay RelativisticCamera::generateGeodesicRay(float x, float y) const 
        {
          Vec3 pRaster(x + 0.5f, y + 0.5f, 0.f);
          Vec3 pCamera = R2C(pRaster);

          Vec3 dcam = ::normalize(static_cast<Vec3>(pCamera));
          Vec3 drot = ::normalize((*C2W)(dcam));
    
          float d1 = drot.z;
          float d2 = drot.y;
          float d3 = drot.x;

          Vec4 plocal(-1.f, d1, d2, d3);

          Vec4 pcontra = tetrad(plocal);

          Mat4 g = metric->g(pos);
          Vec4 pcov;
          float p_arr[4] = {pcontra.t, pcontra.r, pcontra.theta, pcontra.phi};
          float pcov_arr[4] = {0.f, 0.f, 0.f, 0.f};

          for (int i = 0; i < 4; ++i) 
            for (int j = 0; j < 4; ++j) 
              pcov_arr[i] += g[i][j] * p_arr[j];

          pcov.t     = pcov_arr[0];
          pcov.r     = pcov_arr[1];
          pcov.theta = pcov_arr[2];
          pcov.phi   = pcov_arr[3];
          
          return GeodesicRay(pos, pcov);

        }

} // namespace Cam
