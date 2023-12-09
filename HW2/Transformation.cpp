#include "Transformation.h"

Matrix4 Transformation::transform(const Matrix4 &homogenousToTransform)
{
    return this->homogenous*homogenousToTransform;
}