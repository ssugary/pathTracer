#pragma once

#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "Utils/parser.hpp"
#include "ssmath3/paramSet.hpp"
#include <memory>
#include <stack>
#include <string>
#include "ssmath3/ssmath3.hpp"

using namespace ssmath3;

enum AggregateType { LIST = 0, BVH };

namespace Itg 
{
  class Integrator;
}
namespace Cam 
{
  class Camera;
}
namespace Geo 
{
  class Transform;
  struct TransformHash;
  class Scene;
  template <typename T>
  class Texture;
}
namespace Prim 
{
  class Background;
  class Primitive;
}
namespace Luz 
{
  class Light;
  class AreaLight;
}
namespace Mat 
{
  class Material;
}
namespace Fil 
{

}
namespace Sam 
{

}
using namespace Itg;
using namespace Cam;
using namespace Geo;
using namespace Prim;
using namespace Luz;
using namespace Mat;
using namespace Fil;
using namespace Sam;

namespace ssrt {

  struct GraphicsState
  {
    std::shared_ptr<AreaLight> curr_emitter;
    std::shared_ptr<Material> curr_material;  //!< Current material that globally affects all objects.
    std::shared_ptr<Texture<Color>> curr_color_texture;
    std::shared_ptr<Texture<float>> curr_float_texture;
    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<Material>>> mats_lib;      //!< Library of materials.
    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<AreaLight>>> emitter_lib;
    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<Texture<Color>>>> texture_lib;
    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<Texture<float>>>> float_texture_lib;
    bool flip_normals{false};                 //!< When true, we flip the normals
    bool mats_lib_cloned{false};
  };
  struct RenderOptions 
  {
    std::shared_ptr<Background> background;
    std::shared_ptr<Light> envLight;
    std::shared_ptr<Camera> camera;
    std::unique_ptr<Integrator> integrator;
    std::unique_ptr<Scene> scene;
    AggregateType aggregator;
    int num_prims = 4;
    std::vector<std::shared_ptr<Light>> light_sources;
    std::vector<std::shared_ptr<Primitive>> elements;
    std::unordered_map<std::string, ParamSet> setup_params;
    std::unordered_map<std::string, std::vector<std::shared_ptr<Primitive>>> obj_instances;

    std::vector<std::shared_ptr<Primitive>>* curr_instance{nullptr};
  };

  enum class ApiState 
  {
    SETUP_BLOCK=0,
    WORLD_BLOCK,
    INSTANCE_BLOCK,
  };

  class API 
  {

    public:


        static ApiState apiState;
        static std::unique_ptr<RenderOptions> renderOpt;
        static RunningOptions runningOpt;
        
        static std::stack<GraphicsState> savedGS;
        static GraphicsState currentGS;
        static std::stack<Transform> savedTM;  
        static std::unique_ptr<Transform> currentTM;
        
        static std::unordered_map<std::string, Transform> named_coord_sys;
        static std::unordered_map<Transform, std::shared_ptr<const Transform>, TransformHash> transformation_cache;

        static void initEngine(const RunningOptions&);
        static bool checkState(ApiState expected_state, const std::string& tag_name);
        static void render(const ParamSet&);
        static void camera(const ParamSet&);
        static void background(const ParamSet&);
        static void sampler(const ParamSet&);
        static void integrator(const ParamSet&);
        static void film(const ParamSet&);
        static void filter(const ParamSet&);
        static void lookAt(const ParamSet&);
        static void worldBegin(const ParamSet&);
        static void worldEnd(const ParamSet&);
        static void identity(const ParamSet&);
        static void rotate(const ParamSet&);
        static void translate(const ParamSet&);
        static void scale(const ParamSet&);
        static void aggregator(const ParamSet&);
        static void pushCTM(const ParamSet&);
        static void popCTM(const ParamSet&);
        static void pushGS(const ParamSet&);
        static void popGS(const ParamSet&);
        static void object(const ParamSet&);
        static void material(const ParamSet&);
        static void makeNamedMaterial(const ParamSet&);
        static void namedMaterial(const ParamSet&);
        static void lightSource(const ParamSet&);
        static void objInstanceBegin(const ParamSet&);
        static void objInstanceEnd(const ParamSet&);
        static void objInstanceCall(const ParamSet&);
        static void emitter(const ParamSet&);
        static void makeNamedEmitter(const ParamSet&);
        static void namedEmitter(const ParamSet&);
        static void makeNamedTexture(const ParamSet&);
        static void namedTexture(const ParamSet&);
        static std::shared_ptr<const Transform> cacheTransform(const Transform& t);
  };
}; // namespace ssrt

#endif //< APPLICATION_HPP
