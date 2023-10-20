#pragma once
#include "parser.h"

using namespace parser;

struct IntersectionPoint
{
    Vec3D<double> point;
    Face *face;
    Sphere *sphere;
    bool isSphere;
};