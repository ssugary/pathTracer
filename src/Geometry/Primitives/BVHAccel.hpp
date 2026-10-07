#pragma once


#ifndef BVH_ACCEL_HPP
#define BVH_ACCEL_HPP

#include "Geometry/Primitives/AggregatePrimitive.hpp"
#include "Utils/Memory.hpp"

#include <vector>

namespace Prim 
{

    struct BVHPrimitiveInfo 
    {
      BVHPrimitiveInfo() = default;
      BVHPrimitiveInfo(std::size_t p, const Bounds3f& b)
      : pNum(p), bound(b), centroid((b.pMin + b.pMax) * 0.5f) {};

      std::size_t pNum;
      Bounds3f bound;
      Point3 centroid;
    };

    class BVHAccel : public AggregatePrimitive
    {
      public:
          enum SplitMethod 
          { 
            SAH=0,
            HLBVH, 
            MIDDLE, 
            EQUAL_COUNTS
          };
          
          private:
            
            struct BVHBuildNode 
            {
              void initLeaf(int first, int n, const Bounds3f& b)
              {
                firstPOffset = first;
                nPrim = n;
                bound = b;
                children[0] = nullptr;
                children[1] = nullptr;
              }
              void initInterior(int axis, BVHBuildNode* n1, BVHBuildNode* n2)
              {
                children[0] = n1;
                children[1] = n2;
                bound = boundUnion(n1->bound, n2->bound);
                splitAxes = axis;
                nPrim = 0;
              }

              Bounds3f bound;
              BVHBuildNode* children[2];
              int splitAxes;
              int firstPOffset;
              int nPrim;
            };

            const int MAX_PRIMS_PER_NODE;
            const SplitMethod METHOD;
            std::vector<std::shared_ptr<Primitive>> prims;
            MEMO::MemoryArena arena;
            BVHBuildNode* root{nullptr};
            
            public:

            BVHAccel(const std::vector<std::shared_ptr<Primitive>>& p, const int maxPrimsPerNode, SplitMethod m = SAH)
            : MAX_PRIMS_PER_NODE(maxPrimsPerNode), METHOD(m), prims(p), arena(1024 * 1024)
            {
                if(prims.empty())
                    return;
  
                std::vector<BVHPrimitiveInfo> primInfos(prims.size());
                for(std::size_t i{0}; i < prims.size(); ++i)
                  primInfos[i] = BVHPrimitiveInfo(i, prims[i]->objectBound());
                

                int total = 0;
                std::vector<std::shared_ptr<Primitive>> orderedPrims;
                orderedPrims.reserve(prims.size());

                // if(METHOD == SplitMethod::HLBVH)
                //   root = HLBVHBuild(arena, primInfos, 0, prims.size(), &total, orderedPrims);
                // else
                root = recursiveBuild(arena, primInfos, 0, prims.size(), &total, orderedPrims);
                prims.swap(orderedPrims);

              };

            BVHBuildNode* recursiveBuild(MEMO::MemoryArena& arena, 
                                         std::vector<BVHPrimitiveInfo>& pInfo, 
                                         int start, int end, int* totalNodes, 
                                         std::vector<std::shared_ptr<Primitive>>& orderedPrims)
            {
                BVHBuildNode *node = arena.alloc<BVHBuildNode>();
                (*totalNodes)++;

                Bounds3f allBounds;
                for(int i{start}; i < end;++i)
                  allBounds = boundUnion(allBounds, pInfo[i].bound);

                int nPrimitives = end - start;
                if(nPrimitives <= MAX_PRIMS_PER_NODE)
                {
                  int firstPOffset = orderedPrims.size();
                  for(int i{start}; i < end; ++i)
                  {
                    int primNum = pInfo[i].pNum;
                    orderedPrims.push_back(prims[primNum]);
                  }
                  node->initLeaf(firstPOffset, nPrimitives, allBounds);
                  return node;
                }
                else 
                {
                  Bounds3f centroidB;
                  for(int i{start}; i < end; ++i)
                    centroidB = boundUnion(centroidB, pInfo[i].centroid);

                  int dim = centroidB.maximumExtent();
                  int mid = (start + end) / 2;

                  if(centroidB.pMax[dim] == centroidB.pMin[dim])
                  {
                    int firstPOffset = orderedPrims.size();
                    for(int i{start}; i < end; ++i)
                    {
                      int primNum = pInfo[i].pNum;
                      orderedPrims.push_back(prims[primNum]);
                    }
                    node->initLeaf(firstPOffset, nPrimitives, allBounds);
                    return node;
                  }
                  else 
                  {
                    switch(METHOD)
                    {
                      case SplitMethod::MIDDLE:
                        {
                          float pmid = (centroidB.pMin[dim] + centroidB.pMax[dim]) / 2;
                          BVHPrimitiveInfo *midPtr =
                              std::partition(&pInfo[start], 
                                              &pInfo[end - 1] + 1,
                                              [dim, pmid](const BVHPrimitiveInfo &pi) 
                              {
                                  return pi.centroid[dim] < pmid;
                              });
                              mid = midPtr - &pInfo[0];
                              if (mid != start && mid != end)
                                  break;
                              [[fallthrough]];
                        }
                      case SplitMethod::EQUAL_COUNTS:
                        {
                          mid = (start + end) / 2;
                         std::nth_element(&pInfo[start],
                                           &pInfo[mid], 
                                          &pInfo[end - 1] + 1,
                                          [dim](const BVHPrimitiveInfo &a, const BVHPrimitiveInfo &b) 
                          { 
                              return a.centroid[dim] < b.centroid[dim];
                          });
                          
                          break;
                        }
                      case SplitMethod::SAH:
                        {
                            if (nPrimitives <= 2) 
                            {
                                mid = (start + end) / 2;
                                std::nth_element(&pInfo[start], &pInfo[mid], &pInfo[end - 1] + 1,
                                    [dim](const BVHPrimitiveInfo &a, const BVHPrimitiveInfo &b) 
                                      {
                                          return a.centroid[dim] < b.centroid[dim];
                                      });
                            } 
                            else 
                            {
                                constexpr int nBuckets = 12;
                                struct BucketInfo { int count = 0; Bounds3f bounds; };
                                BucketInfo buckets[nBuckets];

                                for (int i = start; i < end; ++i) 
                                {
                                    float offset = (pInfo[i].centroid[dim] - centroidB.pMin[dim]) / 
                                                  (centroidB.pMax[dim] - centroidB.pMin[dim]);
                                    int b = nBuckets * offset;
                                    if (b == nBuckets) b = nBuckets - 1;
                                    buckets[b].count++;
                                    buckets[b].bounds = boundUnion(buckets[b].bounds, pInfo[i].bound);
                                }

                                float cost[nBuckets - 1];
                                for (int i = 0; i < nBuckets - 1; ++i) 
                                {
                                    Bounds3f b0, b1;
                                    int count0 = 0, count1 = 0;
                                    for (int j = 0; j <= i; ++j) 
                                    {
                                        b0 = boundUnion(b0, buckets[j].bounds);
                                        count0 += buckets[j].count;
                                    }
                                    for (int j = i + 1; j < nBuckets; ++j) 
                                    {
                                        b1 = boundUnion(b1, buckets[j].bounds);
                                        count1 += buckets[j].count;
                                    }
                                    cost[i] = 1.0f + (count0 * b0.area() + count1 * b1.area()) / allBounds.area();
                                }

                                float minCost = cost[0];
                                int minCostSplitBucket = 0;
                                for (int i = 1; i < nBuckets - 1; ++i) 
                                {
                                    if (cost[i] < minCost) 
                                    {
                                        minCost = cost[i];
                                        minCostSplitBucket = i;
                                    }
                                }

                                float leafCost = nPrimitives;
                                if (nPrimitives > MAX_PRIMS_PER_NODE || minCost < leafCost) 
                                {
                                    BVHPrimitiveInfo* pmid = std::partition(&pInfo[start], &pInfo[end - 1] + 1,
                                        [=](const BVHPrimitiveInfo &pi)
                                          {
                                            float offset = (pi.centroid[dim] - centroidB.pMin[dim]) / 
                                                          (centroidB.pMax[dim] - centroidB.pMin[dim]);
                                            int b = nBuckets * offset;
                                            if (b == nBuckets) b = nBuckets - 1;
                                            return b <= minCostSplitBucket;
                                          });
                                    mid = pmid - &pInfo[0];
                                } 
                                else 
                                {
                                    int firstPOffset = orderedPrims.size();
                                    for (int i = start; i < end; ++i)
                                        orderedPrims.push_back(prims[pInfo[i].pNum]);
                                    node->initLeaf(firstPOffset, nPrimitives, allBounds);
                                    return node;
                                }
                            }
                            break;
                        }
                      default: 
                        {
                          break;
                        }
                      }
                    node->initInterior(dim, recursiveBuild(arena, pInfo, start, mid, totalNodes, orderedPrims),
                                                  recursiveBuild(arena, pInfo, mid, end, totalNodes, orderedPrims));
                  }
                }
                return node;
            }


            virtual Bounds3f objectBound() const override
            {
              return root ? root->bound : Bounds3f(); 
            }
            virtual bool intersect(const Geo::Ray &r, Geo::SurfaceInteraction *sf) const override
            {
                if (!root) 
                  return false;

                bool hit{false};
                int toVisit{0};
                Vec3 invDir(1.f / r.d.x, 1.f / r.d.y, 1.f / r.d.z);
                int dirIsNeg[3]{ invDir.x < 0, invDir.y < 0, invDir.z < 0 };
                BVHBuildNode* nodesToVisit[64];
                BVHBuildNode* currNode = root;

                while(currNode)
                {
                  if(currNode->bound.intersectP(r, invDir, dirIsNeg))
                  {
                    if(currNode->nPrim > 0)
                    {
                      for(int i{0}; i < currNode->nPrim; ++i)
                        if(prims[currNode->firstPOffset + i]->intersect(r, sf))
                          hit = true;
                      if(toVisit == 0)
                        break;
                      currNode = nodesToVisit[--toVisit];
                    }
                    else 
                    {
                      if (dirIsNeg[currNode->splitAxes]) 
                      {
                          nodesToVisit[toVisit++] = currNode->children[0];
                          currNode = currNode->children[1];
                      } 
                      else 
                      {
                          nodesToVisit[toVisit++] = currNode->children[1];
                          currNode = currNode->children[0];
                      }
                    }
                  }
                  else 
                  {
                    if(toVisit == 0)
                      break;
                    currNode = nodesToVisit[--toVisit];
                  }
                }
                return hit;
            }
            virtual bool intersectP(const Geo::Ray &r) const override
            {
                if (!root) 
                  return false;

                int toVisit{0};
                Vec3 invDir(1.f / r.d.x, 1.f / r.d.y, 1.f / r.d.z);
                int dirIsNeg[3]{ invDir.x < 0, invDir.y < 0, invDir.z < 0 };
                BVHBuildNode* nodesToVisit[64];
                BVHBuildNode* currNode = root;

                while(currNode)
                {
                  if(currNode->bound.intersectP(r, invDir, dirIsNeg))
                  {
                    if(currNode->nPrim > 0)
                    {
                      for(int i{0}; i < currNode->nPrim; ++i)
                        if(prims[currNode->firstPOffset + i]->intersectP(r))
                          return true;
                      if(toVisit == 0)
                        break;
                      currNode = nodesToVisit[--toVisit];
                    }
                    else 
                    {
                      if (dirIsNeg[currNode->splitAxes]) 
                      {
                          nodesToVisit[toVisit++] = currNode->children[0];
                          currNode = currNode->children[1];
                      } 
                      else 
                      {
                          nodesToVisit[toVisit++] = currNode->children[1];
                          currNode = currNode->children[0];
                      }
                    }
                  }
                  else 
                  {
                    if(toVisit == 0)
                      break;
                    currNode = nodesToVisit[--toVisit];
                  }
                }
                return false;
            }

    };
};

#endif //< BVH_ACCEL_HPP
