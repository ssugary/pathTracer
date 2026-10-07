#include "API.hpp"
#include "Cameras/OrthographicCamera.hpp"
#include "Cameras/PerspectiveCamera.hpp"
#include "Geometry/Primitives/BVHAccel.hpp"
#include "Geometry/Primitives/GeometricPrimitive.hpp"
#include "Geometry/Backgrounds/InterpoledBackground.hpp"
#include "Geometry/Primitives/PrimitiveList.hpp"
#include "Geometry/Backgrounds/SingleColorBackground.hpp"
#include "Geometry/Backgrounds/ImageBackground.hpp"
#include "Geometry/Primitives/TransformedPrimitive.hpp"
#include "Geometry/Shapes/Plane.hpp"
#include "Geometry/Shapes/Sphere.hpp"
#include "Geometry/Shapes/Cylinder.hpp"
#include "Integrators/NormalMapIntegrator.hpp"
#include "Geometry/Textures/TextureMap.hpp"
#include "Geometry/Textures/MipMap.hpp"
#include "Geometry/Textures/Texture.hpp"
#include "Integrators/RayCastIntegrator.hpp"
#include "Integrators/SimplePathIntegrator.hpp"
#include "Light/AmbientLight.hpp"
#include "Light/DirectionLight.hpp"
#include "Light/PointLight.hpp"
#include "Materials/BlinnPhongMaterial.hpp"
#include "Materials/FlatMaterial.hpp"
#include "Materials/PBRMaterial.hpp"
#include "Utils/MeshLoader.hpp"
#include "Cameras/Film.hpp"
#include "Filter/BoxFilter.hpp"
#include "Geometry/Transformation/Transform.hpp"
#include "Sampler/Sampler.hpp"
#include "Sampler/PixelSampler.hpp"
#include "Sampler/StratifiedSampler.hpp"
#include "Sampler/GlobalSampler.hpp"
#include "Sampler/HaltonSampler.hpp"
#include "Integrators/Integrator.hpp"
#include "Integrators/BlinnPhongIntegrator.hpp"
#include "MsgSystem/error.hpp"
#include "Cameras/SphericalCamera.hpp"
#include "Filter/GaussianFilter.hpp"
#include "Filter/TriangleFilter.hpp"
#include "Light/SpotLight.hpp"
#include "Light/DiffuseAreaLight.hpp"
#include "Light/EnvironmentLight.hpp"
#include <memory>

namespace ssrt 
{
    RunningOptions API::runningOpt;
    ApiState API::apiState = ApiState::SETUP_BLOCK;
    std::unique_ptr<Transform> API::currentTM = std::make_unique<Transform>(); // Inicia com Matriz Identidade
    std::unordered_map<Transform, std::shared_ptr<const Transform>, TransformHash> API::transformation_cache;

    std::stack<GraphicsState> API::savedGS;
    GraphicsState API::currentGS;
    std::stack<Transform> API::savedTM;  
    
    std::unique_ptr<RenderOptions> API::renderOpt;
    void API::initEngine(const RunningOptions& opt)
    {
        runningOpt = opt;

        apiState = ApiState::SETUP_BLOCK;
        renderOpt = std::make_unique<RenderOptions>();
        MESSAGE("[1] Rendering engine initiated.\n");
    }    

    void API::render(const ParamSet&) 
    {
        // 1. Validação da Cena e Integrador
        if (!renderOpt->scene) 
        {
            ERROR("API::render - A cena não foi inicializada. Certifique-se de que <world_begin> e <world_end> foram chamados.\n");
            return;
        }

        if (!renderOpt->integrator) 
        {
            ERROR("API::render - O integrador não foi inicializado. Verifique a tag <integrator> na cena.\n");
            return;
        }

        // 2. Disparo do pipeline de renderização
        std::cout << "[API] Iniciando Renderização...\n";
        renderOpt->integrator->render(*renderOpt->scene);
        std::cout << "[API] Renderização concluída com sucesso!\n";
    }

    bool API::checkState(ApiState expected_state, const std::string& tag_name) 
    {
        if (apiState != expected_state) 
        {
            std::cerr << "[ERROR] The tag <" << tag_name << "> has called in the wrong state.\n";
            return false;
        }
        return true;
    }
    void API::camera(const ParamSet& ps) 
    {
        if (!checkState(ApiState::SETUP_BLOCK, "camera")) 
            return;
        renderOpt->setup_params["camera"] = ps;
    }

    void API::background(const ParamSet& ps) 
    {
        if (!checkState(ApiState::WORLD_BLOCK, "background")) 
            return;

        std::string bgType = ps.retrieve<std::string>("type", "single");

        if (bgType == "single" || bgType == "single_color" || bgType == "color") 
        {
            Color color = ps.retrieve<Color>("color", Color(0, 0, 0));
            renderOpt->background = std::make_shared<SingleColorBackground>(color);
        } 
        else if (bgType == "interpolated" || bgType == "colors" || bgType == "4_colors") 
        {
            Color bl = ps.retrieve<Color>("bl", Color(0, 0, 0));
            Color tl = ps.retrieve<Color>("tl", Color(1, 1, 1));
            Color tr = ps.retrieve<Color>("tr", Color(1, 1, 1));
            Color br = ps.retrieve<Color>("br", Color(0, 0, 0));
            renderOpt->background = std::make_shared<InterpoledBackground>(bl, tl, tr, br);
        }
        else if (bgType == "image" || ps.contains<std::string>("filename"))
        {
            std::string filename = ps.retrieve<std::string>("filename", "");
            if(filename.empty())
                ERROR("File not found: " + filename + " !");

            int w, h;
            unsigned char* data{};
            if(!ssrt::loadImgBackground(filename, data, &w, &h))
                ERROR("Error on loading file " + filename + " !");
            
            std::shared_ptr<unsigned char> imgData(
                data, [](unsigned char* d) 
                { 
                    if (d) 
                        stbi_image_free(d); 
                    
                }
            );

            renderOpt->background = std::make_shared<ImageBackground>(imgData, w, h);

            renderOpt->envLight = std::make_shared<EnvironmentLight>(imgData, w, h, Color(1.f));
             renderOpt->light_sources.push_back(renderOpt->envLight);
        }
        else 
            ERROR("Invalid Background Type: " + bgType + " !");
    }

    void API::sampler(const ParamSet& ps) 
    {
        if (!checkState(ApiState::SETUP_BLOCK, "sampler")) 
            return;
        renderOpt->setup_params["sampler"] = ps;
    }

    void API::integrator(const ParamSet& ps) 
    {
        if (!checkState(ApiState::SETUP_BLOCK, "integrator"))   
            return;
        renderOpt->setup_params["integrator"] = ps;
    }

    void API::film(const ParamSet& ps) 
    {
        if (!checkState(ApiState::SETUP_BLOCK, "film")) 
            return;
        renderOpt->setup_params["film"] = ps;
    }
    void API::filter(const ParamSet& ps) 
    {
        if (!checkState(ApiState::SETUP_BLOCK, "filter")) 
            return;
        renderOpt->setup_params["filter"] = ps;
    }

    void API::lookAt(const ParamSet& ps) 
    {
        if (!checkState(ApiState::SETUP_BLOCK, "lookat")) 
            return;
        renderOpt->setup_params["lookat"] = ps;
    }

    void API::aggregator(const ParamSet& ps) 
    {
        if (!checkState(ApiState::SETUP_BLOCK, "aggregator")) 
            return;

        std::string type = ps.retrieve<std::string>("type", "bvh");
        renderOpt->aggregator = (type == "list") ? AggregateType::LIST : AggregateType::BVH;
    }

    void API::makeNamedTexture(const ParamSet& ps)
    {
        if (apiState != ApiState::WORLD_BLOCK && apiState != ApiState::INSTANCE_BLOCK) 
            return;

        std::string name = ps.retrieve<std::string>("name", "");
        std::string type = ps.retrieve<std::string>("type", "default");
        std::string mapType = ps.retrieve<std::string>("mapping", "uv");
        std::string data_type = ps.retrieve<std::string>("data_type", "color");

        std::unique_ptr<TextureMapping2D> mapping;
    
        if (mapType == "uv") 
        {
            float su = ps.retrieve<float>("su", 1.f);
            float sv = ps.retrieve<float>("sv", 1.f);
            mapping = std::make_unique<UVMapping2D>(su, sv, 0.f, 0.f);
        } 
        else if (mapType == "spherical") 
            mapping = std::make_unique<SphericalMapping2D>(*currentTM);


        if(data_type == "color")
        {

            std::shared_ptr<Texture<Color>> texture;
    
            if(type == "image")
            {
                std::string filename = ps.retrieve<std::string>("filename", "");
                int w{0}, h{0};
                Color* data{nullptr};
                if(!ssrt::loadImgTexture(filename, data, &w, &h))
                    ERROR("Error on loading file " + filename + " !");
                
                std::string mode = ps.retrieve<std::string>("mode", "black");
            
                WrapMode m;
    
                if(mode == "black")
                    m = WrapMode::BLACK;
                else if(mode == "clamp")
                    m = WrapMode::CLAMP;
                else 
                    m = WrapMode::REPEAT;

                std::string trilinear = ps.retrieve<std::string>("trilinear", "false");
                bool doTrilinear{false};

                if(trilinear == "true" || trilinear == "yes")
                    doTrilinear = true;

                float maxAnisotropy = ps.retrieve<float>("max_anisotropy", 0.f);
                
                
                std::shared_ptr<MIPMap<Color>> mipmap = std::make_shared<MIPMap<Color>>(Point2i(w, h), data, m, doTrilinear, maxAnisotropy);

                delete[] data;
                texture = std::make_shared<ImageTexture<Color>>(std::move(mapping), mipmap);
            }
            else if (type == "constant") 
            {
                Color val = ps.retrieve<Color>("value", Color(1.f));
                texture = std::make_shared<ConstantTexture<Color>>(val);
            }
            else if(type == "bilinear" || type == "bilerp")
            {
                Color bl = ps.retrieve<Color>("bl", Color(0, 0, 0));
                Color tl = ps.retrieve<Color>("tl", Color(1, 1, 1));
                Color tr = ps.retrieve<Color>("tr", Color(1, 1, 1));
                Color br = ps.retrieve<Color>("br", Color(0, 0, 0));

                texture = std::make_shared<BilerpTexture<Color>>(std::move(mapping), bl, tl, tr, br);

            }

            if (currentGS.texture_lib && texture) 
                (*currentGS.texture_lib)[name] = texture;
        }
        else if(data_type == "float")
        {
            std::shared_ptr<Texture<float>> texture;
        
            if (type == "constant") 
            {
                Color val = ps.retrieve<Color>("value", Color(1.f));
                texture = std::make_shared<ConstantTexture<float>>(val[0]);
            }
            else if(type == "image")
            {
                std::string filename = ps.retrieve<std::string>("filename", "");
                int w, h;
                float* data{nullptr};
                if(ssrt::loadImgTexture(filename, data, &w, &h))
                    ERROR("Error on loading file " + filename + " !");
                
                std::string mode = ps.retrieve<std::string>("mode", "black");
            
                WrapMode m;
    
                if(mode == "black")
                    m = WrapMode::BLACK;
                else if(mode == "clamp")
                    m = WrapMode::CLAMP;
                else 
                    m = WrapMode::REPEAT;

                std::string trilinear = ps.retrieve<std::string>("trilinear", "false");
                bool doTrilinear{false};

                if(trilinear == "true" || trilinear == "yes")
                    doTrilinear = true;

                float maxAnisotropy = ps.retrieve<float>("max_anisotropy", 0.f);
            
                std::shared_ptr<MIPMap<float>> mipmap = std::make_shared<MIPMap<float>>(Point2i(w, h), std::move(data), m, doTrilinear, maxAnisotropy);
                
                delete[] data;

                texture = std::make_shared<ImageTexture<float>>(std::move(mapping), mipmap);
            }
            else if(type == "bilinear" || type == "bilerp")
            {
                Color bl = ps.retrieve<Color>("bl", Color(0, 0, 0));
                Color tl = ps.retrieve<Color>("tl", Color(1, 1, 1));
                Color tr = ps.retrieve<Color>("tr", Color(1, 1, 1));
                Color br = ps.retrieve<Color>("br", Color(0, 0, 0));

                texture = std::make_shared<BilerpTexture<float>>(std::move(mapping), bl[0], tl[0], tr[0], br[0]);

            }
            if (currentGS.float_texture_lib && texture) 
                (*currentGS.float_texture_lib)[name] = texture;
        }

    }
    void API::namedTexture(const ParamSet& ps)
    {
        if (apiState != ApiState::WORLD_BLOCK && apiState != ApiState::INSTANCE_BLOCK) 
            return;
        std::string name = ps.retrieve<std::string>("name", "");
        if (currentGS.texture_lib->count(name)) 
        {
            currentGS.curr_color_texture = (*currentGS.texture_lib)[name];
            currentGS.curr_float_texture = nullptr;
        } 
        else if(currentGS.float_texture_lib->count(name))
        {
            currentGS.curr_float_texture = (*currentGS.float_texture_lib)[name];
            currentGS.curr_color_texture = nullptr;
        }
        else 
        {
            ERROR("Named material '" + name + "' not founded.\n");
        }

        
    }
    void API::worldBegin(const ParamSet&) 
    {
        if (!checkState(ApiState::SETUP_BLOCK, "world_begin")) 
            return;
        apiState = ApiState::WORLD_BLOCK;
        
        *currentTM = Transform(); // Id
        transformation_cache.clear(); 
        currentGS = GraphicsState();
        currentGS.mats_lib = std::make_shared<std::unordered_map<std::string, std::shared_ptr<Material>>>();
        currentGS.emitter_lib = std::make_shared<std::unordered_map<std::string, std::shared_ptr<AreaLight>>>();
        currentGS.float_texture_lib  = std::make_shared<std::unordered_map<std::string, std::shared_ptr<Texture<float>>>>();
        currentGS.texture_lib = std::make_shared<std::unordered_map<std::string, std::shared_ptr<Texture<Color>>>>();
        while (!savedTM.empty()) 
            savedTM.pop();
        while (!savedGS.empty()) 
            savedGS.pop();

        renderOpt->elements.clear();
        renderOpt->light_sources.clear();
        MESSAGE("Processing configurations in worldBegin...\n");
        
        std::unique_ptr<Filter> filter;
        if(renderOpt->setup_params.count("filter"))
        {
            const auto& filterPs = renderOpt->setup_params["filter"];
            auto rx = filterPs.retrieve<float>("x_width", 1.f);
            auto ry = filterPs.retrieve<float>("y_width", 1.f);
            auto type = filterPs.retrieve<std::string>("type", "box");

            if(type == "box" || type == "box_filter")
            {
                filter = std::make_unique<BoxFilter>(Vec2(rx, ry));
            }
            else if(type == "triangle" || type == "triangle_filter")
            {
                filter = std::make_unique<TriangleFilter>(Vec2(rx, ry));
            }
            else if(type == "gaussian" || type == "gauss" || type == "gaussian_filter")
            {
                auto alpha = filterPs.retrieve<float>("alpha", 0.f);

                filter = std::make_unique<GaussianFilter>(Vec2(rx, ry), alpha);
            }
            
        }
        else  
        {
            filter = std::make_unique<BoxFilter>(Vec2(1.f, 1.f));
        }
        // float scale = filmPs.retrieve<float>("scale", 1.0);
        std::unique_ptr<Film> film;
        if (renderOpt->setup_params.count("film")) 
        {
            const auto& filmPs = renderOpt->setup_params["film"];
            
            int w_res = filmPs.retrieve<int>("w_res", 800);
            int h_res = filmPs.retrieve<int>("h_res", 600);
            std::string filename = filmPs.retrieve<std::string>("filename", "output.png");
            std::string gamma_corrected = filmPs.retrieve<std::string>("gamma_corrected", "false");
            // float diagonal = filmPs.retrieve<float>("diagonal", 35.0); // 35mm é o padrão
            Bounds2i cropWindow(Point2i(0, 0), Point2i(1, 1));
            runningOpt.outfile = filename;
            film = std::make_unique<Film>(
                Point2i(w_res, h_res), 
                cropWindow, 
                std::move(filter),
                filename
            );

            bool gc{false};
            if(gamma_corrected == "yes" || gamma_corrected == "true")
                gc = true;
            film->gammaC = gc;
        } 
        else 
        {
            Bounds2i defaultCrop(Point2i(0, 0), Point2i(1, 1));
            film = std::make_unique<Film>(
                Point2i(800, 600), 
                defaultCrop,    
                std::move(filter),
                "output.png"
            );
        }
        std::shared_ptr<const Transform> cameraToWorld;
        if (renderOpt->setup_params.count("lookat")) 
        {
            const auto& lookAtPs = renderOpt->setup_params["lookat"];
            Point3 pos = lookAtPs.retrieve<Point3>("look_from", Point3(0, 0, 0));
            Point3 target = lookAtPs.retrieve<Point3>("look_at", Point3(0, 0, -1));
            Vec3 vup = lookAtPs.retrieve<Vec3>("vup", Vec3(0, 1, 0));

            cameraToWorld = cacheTransform(Transform::lookAt(pos, target, vup));
        }
        else 
        {
          cameraToWorld = cacheTransform(Transform());  
        }
        if (renderOpt->setup_params.count("camera")) 
        {
            const auto& camPs = renderOpt->setup_params["camera"];
            std::string camType = camPs.retrieve<std::string>("type", "perspective");

            float shutterOpen = camPs.retrieve<float>("shutter_open", 0.0);
            float shutterClose = camPs.retrieve<float>("shutter_close", 1.0);
            float lensR = camPs.retrieve<float>("lens_radius", 0.0);
            float focalD = camPs.retrieve<float>("focal_distance", 1e30);
            
            float aspect = static_cast<float>(film->fullRes[0]) / static_cast<float>(film->fullRes[1]);
            Bounds2f screenWindow(Point2(-aspect, -1.0), Point2(aspect, 1.0)); 
            
            const Medium* medium = nullptr;

            // AnimatedTransform animCamToWorld(cameraToWorld.get(), 0.0, cameraToWorld.get(), 1.0); 
            
            if (camType == "perspective") 
            {
                float fovy = camPs.retrieve<float>("fovy", 45.0); 
                
                Transform cameraToScreen = Transform::perspective(fovy, 1e-2, 1000.0); 
                renderOpt->camera = std::make_shared<PerspectiveCamera>(
                    cameraToWorld.get(), 
                    cameraToScreen, 
                    screenWindow,
                    film,  
                    medium, 
                    shutterOpen, 
                    shutterClose,
                    lensR, 
                    focalD
                );
            } 
            else if (camType == "orthographic") 
            {
                auto screen_window = camPs.retrieve<Point4>("screen_window", {-5.3, 5.3, -4, 4});
                float l = screen_window.x, r = screen_window.y;
                float b = screen_window.z, t = screen_window.w;

                float winW = r - l;
                float winH = t - b;
                float winAspect = winW / winH;

                if (winAspect > aspect) {
                    float newH = winW / aspect;
                    float cy = (b + t) * 0.5f;
                    b = cy - newH * 0.5f;
                    t = cy + newH * 0.5f;
                } else {
                    float newW = winH * aspect;
                    float cx = (l + r) * 0.5f;
                    l = cx - newW * 0.5f;
                    r = cx + newW * 0.5f;
                }

                Bounds2f scrw(Point2(l, b), Point2(r, t));

                Transform cameraToScreen = Transform::orthographic(1e-2, 1000.0);
                renderOpt->camera = std::make_shared<OrthographicCamera>(
                    cameraToWorld.get(), cameraToScreen, scrw, film, medium,
                    shutterOpen, shutterClose, lensR, focalD
                );
            }
            else if(camType == "spherical")
            {
                std::string mapType = camPs.retrieve<std::string>("mapping", "equirectangular");
                Cam::SphericalCamera::Mapping mapping = Cam::SphericalCamera::Mapping::EQUI_RETANGULAR;
                
                if (mapType == "equal_area") {
                    mapping = Cam::SphericalCamera::Mapping::EQUAL_AREA;
                }

                renderOpt->camera = std::make_shared<Cam::SphericalCamera>(
                    film, 
                    medium, 
                    cameraToWorld.get(), 
                    shutterOpen, 
                    shutterClose, 
                    mapping
                );
            }
        }


        std::shared_ptr<Sam::Sampler> sampler;
        int spp = 1;

        if(renderOpt->setup_params.count("sampler"))
        {
            const auto& samplerPs = renderOpt->setup_params["sampler"];
            
            std::string type = samplerPs.retrieve<std::string>("type", "pixel");
            int nSampledDimensions = samplerPs.retrieve<int>("n_sampled_dimensions", 4);
            
            if(type == "pixel" || type == "pixel_sampler")
            {
                spp = samplerPs.retrieve<int>("samples_per_pixel", 1);
                sampler = std::make_shared<PixelSampler>(spp, nSampledDimensions);
            }
            else if(type == "stratified" || type == "stratified_sampler" || type == "ssampler")
            {
                int x_samples = samplerPs.retrieve<int>("x_samples", 1);
                int y_samples = samplerPs.retrieve<int>("y_samples", 1);
                std::string jitter = samplerPs.retrieve<std::string>("jitter", "false");

                bool jt{false};
                if(jitter == "true" || jitter == "yes")
                    jt = true;

                sampler = std::make_shared<StratifiedSampler>(x_samples, y_samples, jt, nSampledDimensions);
            }
            else if(type == "halton")
            {
                spp = samplerPs.retrieve<int>("samples_per_pixel", 1);

                Bounds2i sampleBounds(Point2i(0, 0), renderOpt->camera->film->fullRes);
                sampler = std::make_shared<HaltonSampler>(spp, sampleBounds);
            }
        }
        else 
            {
                sampler = std::make_shared<PixelSampler>(spp, 1); //< default :p
            }

        if (renderOpt->setup_params.count("integrator")) 
        {
            const auto& itgPs = renderOpt->setup_params["integrator"];
            std::string type = itgPs.retrieve<std::string>("type", "blinn_phong");
            int maxDepth = itgPs.retrieve<int>("depth", 1);

            if (type == "blinn_phong" || type == "blinn") 
            {
                renderOpt->integrator = std::make_unique<BlinnPhongIntegrator>(
                    renderOpt->camera, sampler, maxDepth
                );
            }
            else if(type == "normal_map")
            {
                renderOpt->integrator = std::make_unique<NormalMapIntegrator>(
                    renderOpt->camera, sampler, maxDepth
                );
            }
            else if(type == "ray_cast" || type == "flat")
            {
                renderOpt->integrator = std::make_unique<RayCastIntegrator>(
                    renderOpt->camera, sampler, maxDepth
                );
            }
            else if(type == "path" || type == "simplepath" || type == "simple_path")
            {
                renderOpt->integrator = std::make_unique<SimplePathIntegrator>(
                    renderOpt->camera, sampler, maxDepth, false, false
                );
            }
        }
    }

    void API::worldEnd(const ParamSet& ps) 
    {
        if (!checkState(ApiState::WORLD_BLOCK, "world_end")) return;

        std::cout << "[API] Finalizando carregamento. Primitivas na cena: " 
                  << renderOpt->elements.size() << "\n";

        std::shared_ptr<AggregatePrimitive> aggregate;
        if (renderOpt->aggregator == AggregateType::LIST) 
        {
            aggregate = std::make_shared<PrimitiveList>(renderOpt->elements);
        } 
        else 
        {
            auto maxPrimsPerNode = ps.retrieve<int>("max_prims_per_node", 4);
            aggregate = std::make_shared<BVHAccel>(renderOpt->elements, maxPrimsPerNode);
        }
        
        
        renderOpt->scene = std::make_unique<Scene>(
            aggregate, 
            renderOpt->background,
            renderOpt->light_sources
        );
        renderOpt->scene->lights.envLight = renderOpt->envLight;

        if (renderOpt->integrator && renderOpt->scene) 
        {
            std::cout << "[API] Iniciando Renderização...\n";
            render(ps);
            std::cout << "[API] Renderização concluída com sucesso!\n";
        } 
        else 
        {
            ERROR("Integrador ou Cena não foram inicializados corretamente.\n");
        }


        apiState = ApiState::SETUP_BLOCK;
    }
    void API::identity(const ParamSet&) 
    {
        *currentTM = Transform();
    }

    void API::rotate(const ParamSet& ps) 
    {
        float angle = ps.retrieve<float>("angle", 0.0);
        Point3 axis = ps.retrieve<Point3>("axis", Point3(0, 1, 0));
        
        *currentTM = (*currentTM)(Transform::rotate(angle, axis));
    }

    void API::translate(const ParamSet& ps) 
    {
        Point3 delta = ps.retrieve<Point3>("delta", Point3(0, 0, 0));
        *currentTM = (*currentTM)(Transform::translate(delta));
    }

    void API::scale(const ParamSet& ps) 
    {
        Point3 delta = ps.retrieve<Point3>("delta", {1, 1, 1});
        *currentTM = (*currentTM)(Transform::scale(delta));
    }

    void API::pushCTM(const ParamSet&) 
    {
        savedTM.push(*currentTM);
    }

    void API::popCTM(const ParamSet&) 
    {
        if (savedTM.empty()) 
            return;
        
        *currentTM = savedTM.top();
        savedTM.pop();
    }

    void API::pushGS(const ParamSet&) 
    {
        savedGS.push(currentGS);
    }

    void API::popGS(const ParamSet&) 
    {
        if (savedGS.empty()) 
            return;
        
        currentGS = savedGS.top();
        savedGS.pop();
    }

    void API::material(const ParamSet& ps) 
    {
        if (apiState != ApiState::WORLD_BLOCK && apiState != ApiState::INSTANCE_BLOCK) 
            return;
        std::string type = ps.retrieve<std::string>("type", "blinn_phong");
        std::shared_ptr<Material> mat;
        if(type == "blinn_phong" || type == "blinn")
        {
            auto kd = ps.retrieve<Color>("kd", Color(1.0, 0.0, 1.0));
            auto ks= ps.retrieve<Color>("ks", Color(0.0, 0.0, 0.0));
            auto ka= ps.retrieve<Color>("ka", Color(0.0, 0.0, 0.0));
            auto mirror = ps.retrieve<Color>("mirror", Color(0.0, 0.0, 0.0));
            auto glossiness= ps.retrieve<float>("glossiness", 0);

            mat = std::make_shared<BlinnPhongMaterial>(kd, ks, ka, glossiness, mirror);
        }
        else if(type == "flat")
        {
            auto color = ps.retrieve<Color>("color", {0.f});
            auto mirror = ps.retrieve<Color>("mirror", {0.f});


            mat = std::make_shared<FlatMaterial>(color, mirror);
        }
        else if (type == "pbr" || type == "pbrmaterial" || type == "pbr_material")
        {
            std::shared_ptr<Texture<Color>> kd_texture;
            std::shared_ptr<Texture<Color>> k_texture;
            std::shared_ptr<Texture<Color>> eta_texture;
            std::shared_ptr<Texture<float>> roughness_texture;
            std::shared_ptr<Texture<float>> ior_texture;

            std::string kd_tex = ps.retrieve<std::string>("kd_texture", "");

            if (!kd_tex.empty() && currentGS.texture_lib->count(kd_tex)) 
                kd_texture = (*currentGS.texture_lib)[kd_tex];
            else 
            {
                Color kd_val = ps.retrieve<Color>("kd", Color(1.0, 0.0, 1.0));
                kd_texture = std::make_shared<Geo::ConstantTexture<Color>>(kd_val);
            }

            std::string k_tex = ps.retrieve<std::string>("k_texture", "");

            if (!k_tex.empty() && currentGS.texture_lib->count(k_tex)) 
                k_texture = (*currentGS.texture_lib)[k_tex];
            else 
            {
                Color k_val = ps.retrieve<Color>("k", Color(1.0, 0.0, 1.0));
                k_texture = std::make_shared<Geo::ConstantTexture<Color>>(k_val);
            }
            std::string eta_tex = ps.retrieve<std::string>("eta_texture", "");

            if (!eta_tex.empty() && currentGS.texture_lib->count(eta_tex)) 
                eta_texture = (*currentGS.texture_lib)[eta_tex];
            else 
            {
                Color eta_val = ps.retrieve<Color>("eta", Color(1.0, 0.0, 1.0));
                eta_texture = std::make_shared<Geo::ConstantTexture<Color>>(eta_val);
            }
            std::string roughness_tex = ps.retrieve<std::string>("roughness_texture", "");

            if (!roughness_tex.empty() && currentGS.float_texture_lib->count(roughness_tex)) 
                roughness_texture = (*currentGS.float_texture_lib)[roughness_tex];
            else 
            {
                float roughness_val = ps.retrieve<float>("roughness", 1.f);
                roughness_texture = std::make_shared<Geo::ConstantTexture<float>>(roughness_val);
            }
            std::string ior_tex = ps.retrieve<std::string>("ior_texture", "");

            if (!ior_tex.empty() && currentGS.float_texture_lib->count(ior_tex)) 
                ior_texture = (*currentGS.float_texture_lib)[ior_tex];
            else 
            {
                float ior_val = ps.retrieve<float>("ior", 1.f);
                ior_texture = std::make_shared<Geo::ConstantTexture<float>>(ior_val);
            }

            auto mirror = ps.retrieve<Color>("mirror", Color(0.0f));
            std::string matTypeStr = ps.retrieve<std::string>("mat_type", "matte");
            MatType matType = MatType::MATTE;

            if(matTypeStr == "dielectric") 
                matType = MatType::DIELECTRIC;
            else if(matTypeStr == "conductor") 
                matType = MatType::CONDUCTOR;

            auto normal_map = ps.retrieve<std::string>("normal_map", "");

            std::shared_ptr<Texture<Color>> tex_normal = nullptr;

            if(!normal_map.empty() && currentGS.texture_lib->count(normal_map))
                tex_normal = currentGS.texture_lib->at(normal_map);
            
            mat = std::make_shared<PBRMaterial>(kd_texture, eta_texture, k_texture, roughness_texture, ior_texture, matType, tex_normal, mirror);
        }
 
        currentGS.curr_material = mat;
    }

    void API::makeNamedMaterial(const ParamSet& ps)
    {
        if (apiState != ApiState::WORLD_BLOCK && apiState != ApiState::INSTANCE_BLOCK) 
            return;
        std::string name = ps.retrieve<std::string>("name", "");

        if(name.empty())
            ERROR("Invalid Material Name");
        

        std::string type = ps.retrieve<std::string>("type", "blinn_phong");
        std::shared_ptr<Material> mat;

        if(type == "blinn_phong" || type == "blinn")
        {
            auto kd = ps.retrieve<Color>("kd", Color(1.0, 0.0, 1.0));
            auto ks= ps.retrieve<Color>("ks", Color(0.0, 0.0, 0.0));
            auto ka= ps.retrieve<Color>("ka", Color(0.0, 0.0, 0.0));
            auto mirror = ps.retrieve<Color>("mirror", Color(0.0, 0.0, 0.0));
            auto glossiness= ps.retrieve<float>("glossiness", 0);

            // auto kd = std::make_shared<RGBAlbedoSpectrum>(kd_rgb);
            // auto ks = std::make_shared<RGBAlbedoSpectrum>(ks_rgb);
            // auto ka = std::make_shared<RGBAlbedoSpectrum>(ka_rgb);
            // auto mirror = std::make_shared<RGBAlbedoSpectrum>(mirror_rgb);

            mat = std::make_shared<BlinnPhongMaterial>(kd, ks, ka, glossiness, mirror);
        }
        else if(type == "flat")
        {
            auto color = ps.retrieve<Color>("color", {0.f});
            auto mirror = ps.retrieve<Color>("mirror", {0.f});

            mat = std::make_shared<FlatMaterial>(color, mirror);
        }
        else if (type == "pbr" || type == "pbrmaterial" || type == "pbr_material")
        {

            std::shared_ptr<Texture<Color>> kd_texture;
            std::shared_ptr<Texture<Color>> k_texture;
            std::shared_ptr<Texture<Color>> eta_texture;
            std::shared_ptr<Texture<float>> roughness_texture;
            std::shared_ptr<Texture<float>> ior_texture;

            std::string kd_tex = ps.retrieve<std::string>("kd_texture", "");

            if (!kd_tex.empty() && currentGS.texture_lib->count(kd_tex)) 
                kd_texture = (*currentGS.texture_lib)[kd_tex];
            else 
            {
                Color kd_val = ps.retrieve<Color>("kd", Color(1.0, 0.0, 1.0));
                kd_texture = std::make_shared<Geo::ConstantTexture<Color>>(kd_val);
            }

            std::string k_tex = ps.retrieve<std::string>("k_texture", "");

            if (!k_tex.empty() && currentGS.texture_lib->count(k_tex)) 
                k_texture = (*currentGS.texture_lib)[k_tex];
            else 
            {
                Color k_val = ps.retrieve<Color>("k", Color(1.0, 0.0, 1.0));
                k_texture = std::make_shared<Geo::ConstantTexture<Color>>(k_val);
            }
            std::string eta_tex = ps.retrieve<std::string>("eta_texture", "");

            if (!eta_tex.empty() && currentGS.texture_lib->count(eta_tex)) 
                eta_texture = (*currentGS.texture_lib)[eta_tex];
            else 
            {
                Color eta_val = ps.retrieve<Color>("eta", Color(1.0, 0.0, 1.0));
                eta_texture = std::make_shared<Geo::ConstantTexture<Color>>(eta_val);
            }
            std::string roughness_tex = ps.retrieve<std::string>("roughness_texture", "");

            if (!roughness_tex.empty() && currentGS.float_texture_lib->count(roughness_tex)) 
                roughness_texture = (*currentGS.float_texture_lib)[roughness_tex];
            else 
            {
                float roughness_val = ps.retrieve<float>("roughness", 1.f);
                roughness_texture = std::make_shared<Geo::ConstantTexture<float>>(roughness_val);
            }
            std::string ior_tex = ps.retrieve<std::string>("ior_texture", "");

            if (!ior_tex.empty() && currentGS.float_texture_lib->count(ior_tex)) 
                ior_texture = (*currentGS.float_texture_lib)[ior_tex];
            else 
            {
                float ior_val = ps.retrieve<float>("ior", 1.f);
                ior_texture = std::make_shared<Geo::ConstantTexture<float>>(ior_val);
            }

            auto mirror = ps.retrieve<Color>("mirror", Color(0.0f));

            std::string matTypeStr = ps.retrieve<std::string>("mat_type", "matte");
            MatType matType = MatType::MATTE;

            if(matTypeStr == "dielectric") 
                matType = MatType::DIELECTRIC;
            else if(matTypeStr == "conductor") 
                matType = MatType::CONDUCTOR;

            auto normal_map = ps.retrieve<std::string>("normal_map", "");
            if(normal_map.empty())
                mat = std::make_shared<PBRMaterial>(kd_texture, eta_texture, k_texture, roughness_texture, ior_texture, matType, currentGS.curr_color_texture, mirror);
            else  
                mat = std::make_shared<PBRMaterial>(kd_texture, eta_texture, k_texture, roughness_texture, ior_texture, matType, currentGS.texture_lib->at(normal_map), mirror);
        }
        if (currentGS.mats_lib) 
            (*currentGS.mats_lib)[name] = mat;
        
    }
    void API::namedMaterial(const ParamSet& ps) 
    {
        if (apiState != ApiState::WORLD_BLOCK && apiState != ApiState::INSTANCE_BLOCK) 
            return;
        std::string name = ps.retrieve<std::string>("name", "");
        if (currentGS.mats_lib->count(name)) 
        {
            currentGS.curr_material = (*currentGS.mats_lib)[name];
        } 
        else 
        {
            ERROR("Named material '" + name + "' not founded.\n");
        }
    }


    void API::object(const ParamSet& ps) 
    {
        if (apiState != ApiState::WORLD_BLOCK && apiState != ApiState::INSTANCE_BLOCK) 
        {
            ERROR("Declared <object> is out of world_begin or instance_begin.\n");
            return;
        }

        std::string type = ps.retrieve<std::string>("type", "sphere");
        std::shared_ptr<Primitive> localPrimitive = nullptr;
        std::shared_ptr<Primitive> newPrimitive = nullptr;
        std::shared_ptr<AreaLight> base_emitter = currentGS.curr_emitter;

        if (!currentGS.curr_material) 
            ERROR("No declared material");
        

        if(type == "sphere")
        {
            auto radius = ps.retrieve<float>("radius", 1.f);
            auto center = ps.retrieve<Point3>("center", Point3(0, 0, 0));
            auto zmin = ps.retrieve<float>("zmin", -radius);
            auto zmax = ps.retrieve<float>("zmax", radius);
            auto phimax = ps.retrieve<float>("phimax", 360.f);
            auto shape = std::make_shared<Sphere>(radius, zmin, zmax, phimax, false, false);
            
            Transform objTM = (*currentTM)(Transform::translate(center));
            auto o2w = cacheTransform(objTM);
            auto w2o = cacheTransform(Transform::inverse(objTM));
            std::shared_ptr<AreaLight> localEmitter = nullptr;
            if(base_emitter) 
            {
                localEmitter = base_emitter->clone();
                localEmitter->shape = shape;
                localEmitter->O2W = o2w;
                localEmitter->W2O = w2o;
            }

            localPrimitive = std::make_shared<GeometricPrimitive>(shape, currentGS.curr_material, localEmitter);
            newPrimitive = std::make_shared<TransformedPrimitive>(o2w.get(), w2o.get(), localPrimitive);

        }
        else if (type == "cylinder" || type == "cilindro" || type == "cilinder" || type == "cilynder")
        {
            auto radius = ps.retrieve<float>("radius", 1.f);
            auto center = ps.retrieve<Point3>("center", Point3(0, 0, 0));
            auto zmin = ps.retrieve<float>("zmin", 0.f);
            auto zmax = ps.retrieve<float>("zmax", 0.f);
            if(zmin == zmax)
            {
                auto height = ps.retrieve<float>("height", 2.f * radius);
                zmin = 0.f;
                zmax = height;
            }
            
            auto phimax = ps.retrieve<float>("phimax", 360.f);
            auto shape = std::make_shared<Cylinder>(radius, zmin, zmax, phimax, false, false);
            
            Transform objTM = (*currentTM)(Transform::translate(center));
            auto o2w = cacheTransform(objTM);
            auto w2o = cacheTransform(Transform::inverse(objTM));
            std::shared_ptr<AreaLight> localEmitter = nullptr;
            if(base_emitter) 
            {
                localEmitter = base_emitter->clone();
                localEmitter->shape = shape;
                localEmitter->O2W = o2w;
                localEmitter->W2O = w2o;
            }
            localPrimitive = std::make_shared<GeometricPrimitive>(shape, currentGS.curr_material, localEmitter);
            newPrimitive = std::make_shared<TransformedPrimitive>(o2w.get(), w2o.get(), localPrimitive);
        }
        else if (type == "plane")
        {
            auto point = ps.retrieve<Point3>("point");
            auto normal = ps.retrieve<Normal3>("normal");
            auto shape = std::make_shared<Plane>(point, normal, false, false);
            
            auto o2w = cacheTransform(*currentTM);
            auto w2o = cacheTransform(Transform::inverse(*currentTM));
            
            localPrimitive = std::make_shared<GeometricPrimitive>(shape, currentGS.curr_material);
            newPrimitive = std::make_shared<TransformedPrimitive>(o2w.get(), w2o.get(), localPrimitive);
        }
        else if (type == "triangle_mesh" || type == "trianglemesh")
        {
            std::string rev_order_str = ps.retrieve<std::string>("reverse_vertex_order", "false");
            std::string swap_hand_str      = ps.retrieve<std::string>("swap_handedness", "false");
            std::string cull_str      = ps.retrieve<std::string>("backface_cull", "false");
            bool reverse_order        = (rev_order_str == "true");
            bool backface_cull        = (cull_str == "true");
            bool swap_handedness      = (swap_hand_str == "true");
            auto o2w = cacheTransform(*currentTM);
            auto w2o = cacheTransform(Transform::inverse(*currentTM));
            std::string filename = ps.retrieve<std::string>("filename", "");

            std::vector<std::shared_ptr<Primitive>> meshPrimitives;
            
            if (!filename.empty()) 
            {
                auto triangles = loadOBJ(filename, reverse_order, swap_handedness, backface_cull);
                for (auto& tri : triangles) 
                {
                    std::shared_ptr<AreaLight> localEmitter = nullptr;
                    if(base_emitter) {
                        localEmitter = base_emitter->clone();
                        localEmitter->shape = tri;
                        localEmitter->O2W = o2w;
                        localEmitter->W2O = w2o;
                    }

                    auto geoPrim = std::make_shared<GeometricPrimitive>(tri, currentGS.curr_material, localEmitter);
                    meshPrimitives.push_back(std::make_shared<TransformedPrimitive>(o2w.get(), w2o.get(), geoPrim));
                }
            }
            else 
            {
                int ntriangles = ps.retrieve<int>("ntriangles", 0);
                
                std::vector<int> indices;
                std::vector<Point3> vertices;
                std::vector<Normal3> normals;
                std::vector<Point2> uvs;

                indices = ps.retrieve<std::vector<int>>("indices", {}); 
                vertices = ps.retrieve<std::vector<Point3>>("vertices", {}); 
                normals = ps.retrieve<std::vector<Normal3>>("normals", {}); 
                uvs = ps.retrieve<std::vector<Point2>>("uvs", {}); 

                if (ntriangles > 0 && !indices.empty() && !vertices.empty()) 
                {
                    auto mesh = std::make_shared<TriangleMesh>(
                        ntriangles, 
                        indices.data(), 
                        vertices.size(), 
                        vertices.data(),
                        nullptr, // S
                        normals.empty() ? nullptr : normals.data(),
                        uvs.empty() ? nullptr : uvs.data(),
                        nullptr  // AlphaMask
                    );

                    for (int i = 0; i < ntriangles; ++i) 
                    {
                        auto tri = std::make_shared<Triangle>(reverse_order, swap_handedness, backface_cull, mesh, i);
                        std::shared_ptr<AreaLight> localEmitter = nullptr;
                        if(base_emitter) 
                        {
                            localEmitter = base_emitter->clone();
                            localEmitter->shape = tri;
                            localEmitter->O2W = o2w;
                            localEmitter->W2O = w2o;
                        }
                        auto geoPrim = std::make_shared<GeometricPrimitive>(tri, currentGS.curr_material, localEmitter);
                        meshPrimitives.push_back(std::make_shared<TransformedPrimitive>(o2w.get(), w2o.get(), geoPrim));
                    }
                }
            }
            if (!meshPrimitives.empty()) 
                    newPrimitive = std::make_shared<BVHAccel>(meshPrimitives, 4);
                
            }
            
        if (newPrimitive) 
        {
            if (renderOpt->curr_instance) 
                renderOpt->curr_instance->push_back(newPrimitive);
            else 
                renderOpt->elements.push_back(newPrimitive);
            

            if(newPrimitive->getAreaLight())
                renderOpt->light_sources.push_back(newPrimitive->getAreaLight());
        }
    }

    void API::lightSource(const ParamSet& ps) 
    {
        if (apiState != ApiState::WORLD_BLOCK) {
            ERROR("Lights must be declared in World Block!\n");
            return;
        }

        std::string type = ps.retrieve<std::string>("type", "point");
        std::shared_ptr<Light> light = nullptr;
        auto scale = ps.retrieve<Color>("s", {0, 0, 0});
        auto intensity = ps.retrieve<Color>("i", {0, 0, 0});

        if(type == "ambient")
        {
            light = std::make_shared<AmbientLight>(intensity, scale);
        }
        else if(type == "direction" || type == "directional")
        {
            auto direction = ps.retrieve<Vec3>("direction", {0, 0, 0});

            if(direction == Vec3(0, 0, 0))
            {
                auto from = ps.retrieve<Point3>("from", {0, 0, 0});
                auto to = ps.retrieve<Point3>("to", {0, 0, 0});
                if(from == to)
                {
                    ERROR("Invalid arguments: from, to");
                }
                direction = to - from;
            }

            auto worldRadius = ps.retrieve<float>("world_radius", 1000000);
            light = std::make_shared<DirectionalLight>(intensity, scale, direction, worldRadius);
        }
        else if(type == "point")
        {
            auto pos = ps.retrieve<Point3>("from", {0, 0, 0});
            auto attenuation = ps.retrieve<Vec3>("attenuation", {1, 0, 0});
            light = std::make_shared<PointLight>(intensity, scale, pos, attenuation);
        }
        else if(type == "spot" || type == "spot_light")
        {
            auto pos =  ps.retrieve<Point3>("from", {0, 0, 0});
            auto to = ps.retrieve<Point3>("to", {0, 0, 0});
            auto cutoff = ps.retrieve<float>("cutoff", 0.f);
            auto falloff = ps.retrieve<float>("falloff", 0.f);
            light = std::make_shared<SpotLight>(intensity, scale, pos, to, cutoff, falloff);
        }


        if (light) 
            renderOpt->light_sources.push_back(light);
        
    }
    void API::emitter(const ParamSet& ps)
    {
        if (apiState != ApiState::WORLD_BLOCK && apiState != ApiState::INSTANCE_BLOCK) 
            return;

        std::string type = ps.retrieve<std::string>("type", "default");
        std::shared_ptr<AreaLight> emitter;

        if(type == "diffuse" || type == "diff")
        {
            auto scale = ps.retrieve<Color>("s", {0, 0, 0});
            auto intensity = ps.retrieve<Color>("i", {0, 0, 0});

            auto two_sided = ps.retrieve<std::string>("two_sided", "false");
            bool ts = false;
            if(two_sided == "yes" || two_sided == "true")
                ts = true;

            emitter = std::make_shared<DiffuseAreaLight>(intensity, scale, ts);

        }
        else if(type == "default")
        {
            emitter = nullptr;
        }

        currentGS.curr_emitter = emitter;
    };
    void API::makeNamedEmitter(const ParamSet& ps)
    {
        if (apiState != ApiState::WORLD_BLOCK && apiState != ApiState::INSTANCE_BLOCK) 
            return;

        std::string name = ps.retrieve<std::string>("name", "default");
        std::string type = ps.retrieve<std::string>("type", "default");
        std::shared_ptr<AreaLight> emitter;

        if(type == "diffuse")
        {
            auto scale = ps.retrieve<Color>("s", {0, 0, 0});
            auto intensity = ps.retrieve<Color>("i", {0, 0, 0});

            auto two_sided = ps.retrieve<std::string>("two_sided", "false");
            bool ts = false;
            if(two_sided == "yes" || two_sided == "true")
                ts = true;

            emitter = std::make_shared<DiffuseAreaLight>(intensity, scale, ts);

        }
        else if(type == "default")
        {
            emitter = nullptr;
        }

        if (currentGS.emitter_lib) 
            (*currentGS.emitter_lib)[name] = emitter;
    };

    void API::namedEmitter(const ParamSet& ps)
    {
        if (apiState != ApiState::WORLD_BLOCK && apiState != ApiState::INSTANCE_BLOCK) 
            return;

        std::string name = ps.retrieve<std::string>("name", "");
        if (currentGS.emitter_lib->count(name)) 
        {
            currentGS.curr_emitter = (*currentGS.emitter_lib)[name];
        } 
        else 
        {
            ERROR("Named emitter '" + name + "' not founded.\n");
        }
    };

    void API::objInstanceBegin(const ParamSet& ps) 
    {
        if (!checkState(ApiState::WORLD_BLOCK, "object_instance_begin")) 
            return;
        
        std::string name = ps.retrieve<std::string>("name", "");
        if (name.empty()) 
            return;

        apiState = ApiState::INSTANCE_BLOCK;

        savedTM.push(*currentTM);
        *currentTM = Transform();
        
        renderOpt->obj_instances[name] = std::vector<std::shared_ptr<Primitive>>();
        renderOpt->curr_instance = &renderOpt->obj_instances[name];
    }

    void API::objInstanceEnd(const ParamSet&) 
    {
        if (!checkState(ApiState::INSTANCE_BLOCK, "object_instance_end")) 
            return;
        
        apiState = ApiState::WORLD_BLOCK;
        renderOpt->curr_instance = nullptr;

        if (!savedTM.empty()) 
        {
            *currentTM = savedTM.top();
            savedTM.pop();
        }

    }

    void API::objInstanceCall(const ParamSet& ps) 
    {
        if (apiState != ApiState::WORLD_BLOCK && apiState != ApiState::INSTANCE_BLOCK) 
        {
            ERROR("object_instance_call called in wrong state.");
            return;
        }
        
        std::string name = ps.retrieve<std::string>("name", "");
        if (renderOpt->obj_instances.count(name)) 
        {
            const auto& instance = renderOpt->obj_instances[name];
            if (instance.empty()) 
            {
                WARNING("Instance '" + name + "' is empty. Skipping...");
                return;
            }

            auto primList = std::make_shared<BVHAccel>(instance, 4);

            auto o2w = cacheTransform(*currentTM);
            auto w2o = cacheTransform(Transform::inverse(*currentTM));
            
            auto transformedInstance = std::make_shared<TransformedPrimitive>(o2w.get(), w2o.get(), primList);

            if (primList) 
            {
                if (apiState == ApiState::INSTANCE_BLOCK && renderOpt->curr_instance) 
                {
                    renderOpt->curr_instance->push_back(transformedInstance);
                } 
                else 
                {
                    renderOpt->elements.push_back(transformedInstance);
                }
            }
            
        } else {
            WARNING("Instance '" + name + "' not founded");
        }
    }

    std::shared_ptr<const Transform> API::cacheTransform(const Transform& t) 
    {

        auto it = transformation_cache.find(t);
        if (it != transformation_cache.end())
            return it->second;

        auto cached = std::make_shared<const Transform>(t);
        transformation_cache[t] = cached;
        
        return cached;
    }
};