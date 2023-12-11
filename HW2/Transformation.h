#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include "Matrix4.h"
#include "Vec4.h"

class Transformation
{
    public:
        Vec4 transform(Vec4 &homogenousToTransform);
        Matrix4 transform(const Matrix4 &homogenousToTransform);
        Matrix4 homogenous;
};


#endif