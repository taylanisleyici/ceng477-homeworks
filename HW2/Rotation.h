#ifndef __ROTATION_H__
#define __ROTATION_H__

#include <iostream>
#include "Transformation.h"
#include "Vec3.h"

class Rotation : Transformation
{
public:
    int rotationId;
    double angle;
    Vec3 u;

    Rotation();
    Matrix4 findRotationMatrix();
    Rotation(int rotationId, double angle, double x, double y, double z);
    friend std::ostream &operator<<(std::ostream &os, const Rotation &r);
};

#endif