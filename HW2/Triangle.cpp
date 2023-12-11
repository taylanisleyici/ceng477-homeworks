#include <iomanip>
#include <iostream>
#include "Triangle.h"

// Triangle::Triangle()
// {
//     // this->vertexIds[0] = -1;
//     // this->vertexIds[1] = -1;
//     // this->vertexIds[2] = -1;
//     Triangle(-1, -1, -1);
// }

// Triangle::Triangle(int vid1, int vid2, int vid3)
// {
//     // this->vertexIds[0] = vid1;
//     // this->vertexIds[1] = vid2;
//     // this->vertexIds[2] = vid3;
//     Triangle(vid1, vid2, vid3, Vec3(0, 0, 0));
// }

// Triangle::Triangle(int vid1, int vid2, int vid3, Vec3 normal)
// {
//     this->vertexIds[0] = vid1;
//     this->vertexIds[1] = vid2;
//     this->vertexIds[2] = vid3;
//     this->normal = normal;
// }

Triangle::Triangle(const Triangle &other)
{
    this->vertexIds[0] = other.vertexIds[0];
    this->vertexIds[1] = other.vertexIds[1];
    this->vertexIds[2] = other.vertexIds[2];
    this->normal = other.normal;
    this->vertices[0] = other.vertices[0];
    this->vertices[1] = other.vertices[1];
    this->vertices[2] = other.vertices[2];
}

Triangle::Triangle()
{
    Triangle(Vec4(0, 0, 0, 0), Vec4(0, 0, 0, 0), Vec4(0, 0, 0, 0), -1, -1, -1);
}

Triangle::Triangle(Vec4 v1, Vec4 v2, Vec4 v3, int vid1, int vid2, int vid3)
{
    this->vertices[0] = v1;
    this->vertices[1] = v2;
    this->vertices[2] = v3;
    this->vertexIds[0] = vid1;
    this->vertexIds[1] = vid2;
    this->vertexIds[2] = vid3;
    this->calculateNormal();
}

std::ostream &operator<<(std::ostream &os, const Triangle &t)
{
    os << std::fixed << std::setprecision(0) << "Triangle with vertices (" << t.vertices[0] << ", " << t.vertices[1] << ", " << t.vertices[2] << ")";
    return os;
}

void Triangle::calculateNormal()
{
    Vec4 norm4 = (this->vertices[1] - this->vertices[0]) * (this->vertices[2] - this->vertices[0]).unit();
    this->normal = Vec3(norm4.x, norm4.y, norm4.z);
}