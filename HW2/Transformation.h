#include "Matrix4.h"

class Transformation
{
    public:
        Matrix4 transform(const Matrix4 &homogenousToTransform);
        Matrix4 homogenous;
};