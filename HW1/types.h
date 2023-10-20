#pragma once
#include "parser.h"

using namespace parser;

typedef Vec3D<double> Point;

struct IntersectionPoint
{
    Point point;
    Face *face;
    Sphere *sphere;
    bool isSphere;
};