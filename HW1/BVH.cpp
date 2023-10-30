#include "BVH.h"

using namespace parser;
using namespace std;

// NODE FUNCTIONS

std::vector<BVHLeaf *> leafs;

bool BVHNode::isLeaf() const
{
    return false;
}

pair<Vec3D<double>, Vec3D<double>> BVHNode::getMinMax() const
{
    return pair<Vec3D<double>, Vec3D<double>>(minVertex, maxVertex);
}

BVHNode::BVHNode()
{
    node1 = nullptr;
    node2 = nullptr;
}

IntersectionPoint BVHNode::intersect(const Ray3D &ray, const Scene &scene) const
{
    Vec3D<double> t1 = (this->minVertex - ray.o);
    t1.x = t1.x / ray.d.x;
    t1.y = t1.y / ray.d.y;
    t1.z = t1.z / ray.d.z;
    Vec3D<double> t2 = (this->maxVertex - ray.o);
    t2.x = t2.x / ray.d.x;
    t2.y = t2.y / ray.d.y;
    t2.z = t2.z / ray.d.z;
    Vec3D<double> minVec;
    Vec3D<double> maxVec;
    minVec = Vec3D<double>(min(t1.x, t2.x), min(t1.y, t2.y), min(t1.z, t2.z));
    maxVec = Vec3D<double>(max(t1.x, t2.x), max(t1.y, t2.y), max(t1.z, t2.z));
    double tMin = max(minVec.x, max(minVec.y, minVec.z));
    double tMax = min(maxVec.x, min(maxVec.y, maxVec.z));

    IntersectionPoint returnPoint;
    if (tMin > tMax || tMin < EPSILON)
    {
        returnPoint.distance = numeric_limits<double>::max();
        return returnPoint;
    }
    else
    {
        IntersectionPoint ret1Point, ret2Point;
        if (node1 == nullptr)
        {
            ret1Point.distance = numeric_limits<double>::max();
        }
        else
        {
            ret1Point = node1->intersect(ray, scene);
        }
        if (node2 == nullptr)
        {
            ret2Point.distance = numeric_limits<double>::max();
        }
        else
        {
            ret2Point = node2->intersect(ray, scene);
        }
        if (ret1Point.distance < ret2Point.distance)
        {
            return ret1Point;
        }
        return ret2Point;
    }
}

BVHNode::BVHNode(BVHNode *node1, BVHNode *node2)
{
    minVertex.x = min(node1->minVertex.x, node2->minVertex.x);
    minVertex.y = min(node1->minVertex.y, node2->minVertex.y);
    minVertex.z = min(node1->minVertex.z, node2->minVertex.z);
    maxVertex.x = max(node1->maxVertex.x, node2->maxVertex.x);
    maxVertex.y = max(node1->maxVertex.y, node2->maxVertex.y);
    maxVertex.z = max(node1->maxVertex.z, node2->maxVertex.z);
    this->node1 = node1;
    this->node2 = node2;
}

BVHNode *buildTree(std::vector<BVHLeaf *> &leafs, int start, int end)
{
    if (start == end)
    {
        return leafs[start];
    }
    else if (start + 1 == end)
    {
        return new BVHNode(leafs[start], leafs[end]);
    }
    else
    {
        int mid = (start + end) / 2;
        BVHNode *node1 = buildTree(leafs, start, mid);
        BVHNode *node2 = buildTree(leafs, mid + 1, end);
        return new BVHNode(node1, node2);
    }
}

// LEAF FUNCTIONS

IntersectionPoint BVHLeaf::intersect(const Ray3D &ray, const Scene &scene) const
{
    if (this->isSphere())
    {
        return raySphereIntersection(ray, *(this->sphere), scene);
    }
    return rayTriangleIntersection(ray, *(this->triangle), scene);
}

bool BVHLeaf::isSphere() const
{
    return _isSphere;
}

bool BVHLeaf::isLeaf() const
{
    return true;
}

BVHLeaf::BVHLeaf(Sphere &sphere, Scene &scene)
{
    _isSphere = true;
    this->sphere = &sphere;
    triangle = nullptr;
    this->minVertex = scene.vertex_data[sphere.center_vertex_id - 1] - Vec3D<double>(sphere.radius, sphere.radius, sphere.radius);
    this->maxVertex = scene.vertex_data[sphere.center_vertex_id - 1] + Vec3D<double>(sphere.radius, sphere.radius, sphere.radius);
}

BVHLeaf::BVHLeaf(Triangle &triangle, Scene &scene)
{
    _isSphere = false;
    this->sphere = nullptr;
    this->triangle = &triangle;
    Vec3D<double> *v0, *v1, *v2;
    v0 = &(scene.vertex_data[triangle.indices.v0_id - 1]);
    v1 = &(scene.vertex_data[triangle.indices.v1_id - 1]);
    v2 = &(scene.vertex_data[triangle.indices.v2_id - 1]);
    minVertex.x = std::min(v0->x, std::min(v1->x, v2->x));
    minVertex.y = std::min(v0->y, std::min(v1->y, v2->y));
    minVertex.z = std::min(v0->z, std::min(v1->z, v2->z));
    maxVertex.x = std::max(v0->x, std::max(v1->x, v2->x));
    maxVertex.y = std::max(v0->y, std::max(v1->y, v2->y));
    maxVertex.z = std::max(v0->z, std::max(v1->z, v2->z));
}

Triangle *BVHLeaf::getTriangle() const
{
    return this->triangle;
}

Sphere *BVHLeaf::getSphere() const
{
    return this->sphere;
}
