#include "Ray3D.h"

Ray3D::Ray3D()
{
    this->o = Vec3D<double>();
    this->d = Vec3D<double>();
}

Ray3D::Ray3D(Vec3D<double> o, Vec3D<double> d)
{
    this->o = o;
    this->d = d;
}

