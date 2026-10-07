#include "MeshLoader.hpp"
#include <STBI/stb_image.h>
#include <optional>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tinyobj/tiny_obj_loader.h"

#include <iostream>
#include <map>

struct VertexKey {
    int v, n, t;
    
    bool operator<(const VertexKey& other) const 
    {
        if (v != other.v) 
            return v < other.v;
        if (n != other.n) 
            return n < other.n;

        return t < other.t;
    }
};


namespace ssrt 
{

    std::vector<std::shared_ptr<Geo::Triangle>> loadOBJ(const std::string& filepath, 
                                                                        bool reverseOrientation,
                                                                        bool swapHandedness,
                                                                        bool backfaceCull) 
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        bool success = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str(), nullptr, true);

        if (!warn.empty()) 
            std::cout << "OBJ WARN: " << warn << std::endl;
        if (!err.empty())  
            std::cerr << "OBJ ERR: " << err << std::endl;
        if (!success)      
            return {};


        std::vector<Point3> P;
        std::vector<Normal3> N;
        std::vector<Point2> UV;
        std::vector<int> vertexIndices;

        std::map<VertexKey, int> vertexCache;
        bool hasNormals = false;
        bool hasUVs = false;

        for (const auto& shape : shapes) 
        {
            for (const auto& index : shape.mesh.indices) 
            {
                VertexKey key{index.vertex_index, index.normal_index, index.texcoord_index};
                
                if (vertexCache.find(key) == vertexCache.end()) 
                {
                    int newIndex = static_cast<int>(P.size());
                    vertexCache[key] = newIndex;

                    Point3 p(attrib.vertices[3 * key.v + 0], 
                                attrib.vertices[3 * key.v + 1], 
                                attrib.vertices[3 * key.v + 2]);

                    P.push_back(p);

                    if (key.n >= 0) 
                    {
                        hasNormals = true;
                        Normal3 n(attrib.normals[3 * key.n + 0], 
                                    attrib.normals[3 * key.n + 1], 
                                    attrib.normals[3 * key.n + 2]);
                        N.push_back(n);
                    } else 
                        N.push_back(Normal3(0));  
                    

                    if (key.t >= 0) 
                    {
                        hasUVs = true;
                        Point2 uv(attrib.texcoords[2 * key.t + 0], 
                                        attrib.texcoords[2 * key.t + 1]);
                        UV.push_back(uv);
                    } else 
                        UV.push_back(Point2(0)); 
                    
                }
                
                vertexIndices.push_back(vertexCache[key]);
            }
        }

        int nTriangles = static_cast<int>(vertexIndices.size() / 3);
        int nVertices = static_cast<int>(P.size());

        auto mesh = std::make_shared<Geo::TriangleMesh>
        (
            nTriangles,
            vertexIndices.data(),
            nVertices,
            P.data(),
            nullptr, 
            hasNormals ? N.data() : nullptr, 
            hasUVs ? UV.data() : nullptr,    
            nullptr  
        );

        std::vector<std::shared_ptr<Geo::Triangle>> triangles;
        triangles.reserve(nTriangles);

        for (int i = 0; i < nTriangles; ++i) 
        {
            triangles.push_back(std::make_shared<Geo::Triangle>
                (
                    reverseOrientation, 
                    swapHandedness, 
                    backfaceCull,
                    mesh, 
                    i
                ));
        }

        return triangles;
    }

    bool loadImgBackground(const std::string& filename, unsigned char*& data, int* width, int* height, int c)
    {
        int channels;
        data = stbi_load(filename.c_str(), width, height, &channels, c);
    
        if(!data) 
        {
            std::cerr << "ERRO STB: " << stbi_failure_reason() << " | Arquivo: " << filename << std::endl;
            return false;
        }

        return true;
    }
    bool loadImgTexture(const std::string& filename, Color*& data, int* width, int* height, int c)
    {
        int channels;
        unsigned char* raw;
        raw = stbi_load(filename.c_str(), width, height, &channels, c);
        
        if(!raw) 
        {
            std::cerr << "ERRO STB: " << stbi_failure_reason() << " | Arquivo: " << filename << std::endl;
            return false;
        }
        
        data = new Color[(*width) * (*height)];

        for(int i{0}; i < (*width) * (*height); ++i)
            data[i] = Color(raw[i * 3] / 255.f, raw[i * 3 + 1] / 255.f, raw[i * 3 + 2] / 255.f);
        
        stbi_image_free(raw);
        return true;
    }
    bool loadImgTexture(const std::string& filename, float*& data, int* width, int* height, int c)
    {
        int channels;
        data = stbi_loadf(filename.c_str(), width, height, &channels, c);
        
        if(!data) 
        {
            std::cerr << "ERRO STB: " << stbi_failure_reason() << " | Arquivo: " << filename << std::endl;
            return false;
        }

        return true;
    }
}; 