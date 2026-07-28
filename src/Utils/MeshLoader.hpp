#ifndef TRIANGLE_MESH_LOADER_HPP
#define TRIANGLE_MESH_LOADER_HPP

#include "Geometry/Shapes/Triangle.hpp"
#include <memory>
#include <vector>


namespace ssrt 
{
    std::vector<std::shared_ptr<Geo::Triangle>> loadOBJ(
        const std::string& filepath, 
        bool reverseOrientation = false,
        bool swapHandedness = false
    );

    bool loadImgBackground(const std::string& filename, unsigned char*& data, int* width, int* height, int c=3);

} //< namespace ssrt


#endif //< TRIANGLE_MESH_LOADER_HPP