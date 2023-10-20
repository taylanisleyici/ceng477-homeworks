#ifndef __RAY3D_H__
#define __RAY3D_H__

#include "Vec3D.h"

class Ray3D
{
    public:
        Vec3D<double> o;
        Vec3D<double> d;
        Ray3D();
        Ray3D(Vec3D<double> o, Vec3D<double> d);
};

#endif