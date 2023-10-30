#ifndef __BVH__
#define __BVH__

#include "Ray3D.h"
#include "types.h"
#include "Vec3D.h"
#include "parser.h"
#include "math.h"
#include "helpers.h"

#include <utility>


class BVHNode
{
public:
    BVHNode();
    BVHNode(BVHNode *node1, BVHNode *node2);
    // ~BVHNode();
    virtual bool isLeaf() const;
    virtual std::pair<Vec3D<double>, Vec3D<double>> getMinMax() const;
    virtual IntersectionPoint intersect(const Ray3D &ray, const parser::Scene &scene) const;
    Vec3D<double> minVertex, maxVertex;
    BVHNode *node1, *node2;
private:
    
};

class BVHLeaf: public BVHNode
{
public:
    bool isLeaf() const override;
    BVHLeaf(parser::Sphere &sphere, parser::Scene &scene);
    BVHLeaf(parser::Triangle &triangle, parser::Scene &scene);
    bool isSphere() const;
    parser::Triangle *getTriangle() const;
    parser::Sphere *getSphere() const;
    IntersectionPoint intersect(const Ray3D &ray, const parser::Scene &scene) const override;
private:
    bool _isSphere;
    parser::Triangle *triangle;
    parser::Sphere *sphere;
};

BVHNode *buildTree(std::vector<BVHLeaf*> &leafs, int start, int end);

extern std::vector<BVHLeaf*> leafs;

#endif