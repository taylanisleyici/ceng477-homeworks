#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

#include "Vec3.h"
#include "Vec4.h"

class Triangle
{
public:

    // Vec3 normal;
    // Triangle();
    // Triangle(int vid1, int vid2, int vid3);
    // Triangle(int vid1, int vid2, int vid3, Vec3 normal);
    bool isSolid;
    Vec4 vertices[3];
    int vertexIds[3];
    Vec3 normal;
    Triangle();
    Triangle(Vec4 v1, Vec4 v2, Vec4 v3, int vid1, int vid2, int vid3, bool isSolid);
    Triangle(const Triangle &other);
    void calculateNormal();
    friend std::ostream &operator<<(std::ostream &os, const Triangle &t);
};

#endif