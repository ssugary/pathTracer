#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "Geometry/Shapes/Shape.hpp"
#include "Geometry/Textures/Texture.hpp"
#include <memory>
#include <vector>

namespace Geo 
{

    struct TriangleMesh 
    {
        TriangleMesh(int nTriangles, const int *vertexIndices, int nVertices, 
                     const Point3 *P, const Vec3 *S, const Normal3 *N, const Point2 *UV,
                     const std::shared_ptr<Texture<float>> &alphaMask)
                    : nTriangles(nTriangles), nVertices(nVertices), vertexIndices(vertexIndices, vertexIndices + 3 * nTriangles),
                      alphaMask(alphaMask) 
        {
            p.reset(new Point3[nVertices]);
            for(int i{0}; i < nVertices; ++i)
                p[i] = P[i];
            if (UV) 
            {
                uv.reset(new Point2[nVertices]);
                memcpy((void*)uv.get(), UV, nVertices * sizeof(Point2));
            }
            if (N) {
                n.reset(new Normal3[nVertices]);
                for (int i = 0; i < nVertices; ++i)
                    n[i] = N[i];
            }
            if (S) {
                s.reset(new Vec3[nVertices]);
                for (int i = 0; i < nVertices; ++i)
                    s[i] = S[i];
            }
        };

        const int nTriangles;
        const int nVertices;
        std::vector<int> vertexIndices;
        std::unique_ptr<Point3[]> p;
        std::unique_ptr<Normal3[]> n;
        std::unique_ptr<Vec3[]> s;
        std::unique_ptr<Point2[]> uv;
        std::shared_ptr<Texture<float>> alphaMask;
    };

    class Triangle : public Shape
    {
        private:

            std::shared_ptr<TriangleMesh> mesh;
            const int *v;

        public:

            Triangle(bool reverseOrientation, bool tSwapHandedness, 
                     const std::shared_ptr<TriangleMesh> &mesh, int triNumber);
            
            void getUVs(Point2 uvs[3]) const;
            bool intersect(const Ray &r, float *tHit, SurfaceInteraction *sf, bool testAlphaTexture = true) const override;
            bool intersectP(const Ray &r, bool testAlphaTexture = true) const override;
            // Interaction sample(const Point2& u, float* pdf) const override;
            float area() const override;
            Bounds3f objectBound() const override;

    };
}

#endif //< TRIANGLE_HPP