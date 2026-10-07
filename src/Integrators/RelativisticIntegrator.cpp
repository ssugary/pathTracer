#include "RelativisticIntegrator.hpp"
#include "Geometry/Solver/RK4Solver.hpp"
#include <thread>

namespace Itg   
{

  
  RelativisticIntegrator::RelativisticIntegrator(std::shared_ptr<Cam::RelativisticCamera> cam,
                                                  std::shared_ptr<Sam::Sampler> sampler, 
                                                  std::shared_ptr<Metric> metric, 
                                                  std::unique_ptr<Geo::GeodesicSolver> solver,
                                                  int maxSteps, float rEscape, float dLambda, float rDiskOut)
  : Integrator(), cam(std::move(cam)), sampler(std::move(sampler)), metric(std::move(metric)), solver(std::move(solver)),
    maxSteps(maxSteps), rEscape(rEscape), dLambda(dLambda), rDiskOut(rDiskOut) 
    {
      if(!this->solver) 
        this->solver = std::make_unique<Geo::RK4Solver>();
    };   
 
  float RelativisticIntegrator::computeIscoRadius() const
  {
    float M = metric->getMass();
    float a = metric->getSpin();

    if(M <= 0.f) 
      return 0.f;

    float aStar = std::clamp(a / M, 0.f, 0.9999f);
    float z1    = 1.f + std::cbrt(1.f - aStar * aStar) * (std::cbrt(1.f + aStar) + std::cbrt(1.f - aStar));
    float z2    = std::sqrt(3.f * aStar * aStar + z1 * z1);
    
    float rIsco = M * (3.f + z2 - std::sqrt((3.f - z1) * (3.f + z1 + 2.f * z2)));

    return rIsco;
  }

  float RelativisticIntegrator::computeRedshift(const GeodesicRay& ray, float rDisk) const
  {
    float M = metric->getMass();
    float a = metric->getSpin();

    float sqrtM = std::sqrt(M);
    float omega = sqrtM / (std::pow(rDisk, 1.5f) + a * sqrtM);

    Mat4 gMat = metric->g(Point4(0.f, rDisk, PI_OVER_TWO, 0.f));
    double gtt = gMat[0][0];
    double gtp = gMat[0][3];
    double gpp = gMat[3][3];

    double denom = -(gtt + 2. * omega * gtp + omega * omega * gpp);

    if(denom <= 1e-12) 
      return 1.0f;

    double ut = 1. / std::sqrt(denom);

    double Eem  = -(ray.p.t + omega * ray.p.phi) * ut;
    double Eobs = -ray.p.t; 

    if(Eem <= 1e-12) 
      return 1.f;

    return static_cast<float>(Eobs / Eem);
  }

  Vec3 RelativisticIntegrator::computeEscapeDirection(const Point4& x, const Geo::PhaseSpaceDerivatives& derivs) const
  {
    float r     = x.r;
    float theta = x.theta;
    float phi   = x.phi;

    float rDot     = derivs.dxdl.r;
    float thetaDot = derivs.dxdl.theta;
    float phiDot   = derivs.dxdl.phi;

    float sinT = std::sin(theta);
    float cosT = std::cos(theta);
    float sinP = std::sin(phi);
    float cosP = std::cos(phi);

    Vec3 rHat(sinT * cosP, cosT, sinT * sinP);
    Vec3 thetaHat(cosT * cosP, -sinT, cosT * sinP);
    Vec3 phiHat(-sinP, 0.f, cosP);

    Vec3 v = rDot * rHat + (r * thetaDot) * thetaHat + (r * sinT * phiDot) * phiHat;

    return ::normalize(v);
  }

  RelativisticIntegrator::HitResult RelativisticIntegrator::li(const GeodesicRay& gRay, const Scene& scene, Sam::Sampler& sampler) const
  {
    HitResult result;
    GeodesicRay ray = gRay;

    float rHorizon = metric->eventHorizonRadius();
    float rHorizonStop = rHorizon * 1.001f;
    float stepSize = dLambda;
    float rIsco = computeIscoRadius();
    Point4 prevX = ray.x;

    for(int step{0}; step < maxSteps; ++step)
    {

      if(!std::isfinite(ray.x.y) || !std::isfinite(ray.x.z) || !std::isfinite(ray.p.t)
         || ray.x.y <= rHorizonStop || metric->isInsideHorizon(ray.x)) 
      {
        result.type = HitType::CAPTURED;
        result.L = Color(0.f);

        return result;
      }

      if(ray.x.y >= rEscape)
      {
        Geo::PhaseSpaceDerivatives derivs = metric->evaluate(ray.x, ray.p);
        result.type = HitType::ESCAPED;
        result.escapeDir = computeEscapeDirection(ray.x, derivs);
        
        result.L = scene.background->sample(result.escapeDir);

        return result;
      }

      StepResult res = solver->step(ray, *metric, stepSize);

      GeodesicRay nextRay = res.ray;
      
      float prevThetaOffset = prevX.z - PI_OVER_TWO;
      float currThetaOffset = nextRay.x.z - PI_OVER_TWO;
      
      if (prevThetaOffset * currThetaOffset <= 0.f)
      {
        float denom = std::abs(currThetaOffset - prevThetaOffset);
        float t = (denom > EPSILON) ? std::abs(prevThetaOffset) / denom : 0.5f;

        float rDisk = ::lerp(t, prevX.y, nextRay.x.y);

        if (rDisk >= rIsco && rDisk <= rDiskOut)
        {
          float g = computeRedshift(nextRay, rDisk);

          Color I_emit(1.f, 0.8f, 0.4f); 
          result.type = HitType::DISK;
          result.L = std::pow(g, 4.f) * I_emit;

          return result;
        }
      }

      prevX = ray.x;
      ray = nextRay;
      
      stepSize = res.recommendedNextStep > 0.f ? res.recommendedNextStep : dLambda;
    }

    result.type = HitType::MAX_STEPS;
    result.L = Color(0.f);

    return result;
  }


  void RelativisticIntegrator::render(const Scene& scene) 
  {
    if (!cam || !cam->film || !sampler) 
    {
      std::cerr << "[Erro Render] Camera, Film ou Sampler não foram inicializados!\n";
      return;
    }

    preprocess(scene);

    Bounds2i bounds = cam->film->getSampleBounds();

    auto w = bounds.pMax.x;
    auto h = bounds.pMax.y;

    unsigned int nThreads = std::min((unsigned int)std::max(1, h - bounds.pMin.y), 
                                     std::max(1u, std::thread::hardware_concurrency()));
    std::vector<std::thread> workers;

    auto renderRows = [&](int rowStart, int rowEnd, int threadSeed)
    {
      std::unique_ptr<Sam::Sampler> localSampler = sampler->clone(threadSeed);

      for(int j{rowStart}; j < rowEnd; ++j)
        for(int i{bounds.pMin.x}; i < w; ++i)
        {
          localSampler->startPixel(Point2i(i, j));
          do
          {
            Point2 jitter = localSampler->get2D();
            float px = i + jitter.x;
            float py = j + jitter.y;

            GeodesicRay ray = cam->generateGeodesicRay(px, py);
            HitResult hit = li(ray, scene, *localSampler);

            cam->film->addSample(Point2(i, j), hit.L);
          } while(localSampler->startNextSample());
        }
    };

    int rowsPerThread = std::max(1, (h - bounds.pMin.y) / static_cast<int>(nThreads));
    int row = bounds.pMin.y;

    for(unsigned int t{0}; t < nThreads; ++t)
    {
      int rowEnd = (t == nThreads - 1) ? h : std::min(h, row + rowsPerThread);
      workers.emplace_back(renderRows, row, rowEnd, static_cast<int>(t));
      row = rowEnd;
    }
    
    for(auto& wId : workers) 
    {
      if(wId.joinable())
        wId.join();
    }

    cam->film->writeImage();
  }

  void RelativisticIntegrator::preprocess(const Scene& scene) 
  {

  }

} // namespace Itg
