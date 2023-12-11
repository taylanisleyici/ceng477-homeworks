#include "Transformation.h"
#include "Helpers.h"

Matrix4 Transformation::transform(const Matrix4 &homogenousToTransform)
{
    return this->homogenous*homogenousToTransform;
}

Vec4 Transformation::transform(Vec4 &homogenousToTransform)
{
    Vec4 result = this->homogenous*homogenousToTransform;
    result.colorId = homogenousToTransform.colorId;
    return result;
}