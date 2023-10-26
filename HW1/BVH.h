#pragma once

#include "Ray3D.h"
#include "types.h"
#include "Vec3D.h"
#include "parser.h"
#include "math.h"

#include <utility>


using namespace parser;

class BVHNode
{
public:
    bool isLeaf() const;
    pair<Vec3D<double>, Vec3D<double>> getMinMax() const;
private:
    Vec3D<double> minVertex, maxVertex;
};

class BVHLeaf: public BVHNode
{
public:
    BVHLeaf(const Sphere &sphere, const Scene &scene);
    BVHLeaf(const Triangle &triangle, const Scene &scene);
    bool isSphere() const;
    Triangle *getTriangle() const;
    Sphere *getSphere() const;
private:
    bool _isSphere;
    Triangle *triangle;
    Sphere *sphere;
}