#include "BVH.h"

using namespace parser;
using namespace std;

// NODE FUNCTIONS

bool BVHNode::isLeaf() const override
{
    return false;
}

pair<Vec3D<double>, Vec3D<double>> BVHNode::getMinMax() const
{
    return pair<Vec3D<double>, Vec3D<double>>(minVertex, maxVertex);
}



// LEAF FUNCTIONS

BVHLeaf::isSphere() const
{
    return _isSphere;
}

BVHLeaf::isLeaf() const override
{
    return true;
}

BVHLeaf::BVHLeaf(const Sphere &sphere, const Scene &scene)
{
    _isSphere = true;
    this->sphere = &sphere;
    triangle = nullptr;
    this->minVertex = scene.vertex_data[sphere.center_vertex_id - 1] - Vec3D<double>(sphere.radius, sphere.radius, sphere.radius);
    this->maxVertex = scene.vertex_data[sphere.center_vertex_id - 1] + Vec3D<double>(sphere.radius, sphere.radius, sphere.radius);
}

BVHLeaf::BVHLeaf(const Triangle &triangle, const Scene &scene)
{
    _isSphere = false;
    this->sphere = nullptr;
    this->triangle = &triangle;
    Vec3D<double> *v0, *v2, *v3;
    v0 = &(scene.vertex_data[triangle.indices.v0_id - 1]);
    v1 = &(scene.vertex_data[triangle.indices.v1_id - 1]);
    v2 = &(scene.vertex_data[triangle.indices.v2_id - 1]);
    minVertex.x = std::min(v0->x, std::min(v2->x, v3->x));
    minVertex.y = std::min(v0->y, std::min(v2->y, v3->y));
    minVertex.z = std::min(v0->z, std::min(v2->z, v3->z));
    maxVertex.x = std::max(v0->x, std::max(v2->x, v3->x));
    maxVertex.y = std::max(v0->y, std::max(v2->y, v3->y));
    maxVertex.z = std::max(v0->z, std::max(v2->z, v3->z));
}

Triangle *BVHLeaf::getTriangle() const
{
    return this->triangle;
}

Sphere *BVHLeaf::getSphere() const
{
    return this->sphere;
}