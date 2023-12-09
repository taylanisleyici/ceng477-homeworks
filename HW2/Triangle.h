#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

#include "Vec3.h"

class Triangle
{
public:
    int vertexIds[3];
    Vec3 normal;
    Triangle();
    Triangle(int vid1, int vid2, int vid3);
    Triangle(int vid1, int vid2, int vid3, Vec3 normal);
    Triangle(const Triangle &other);
    friend std::ostream &operator<<(std::ostream &os, const Triangle &t);
};

#endif