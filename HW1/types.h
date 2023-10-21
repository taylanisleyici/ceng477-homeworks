#pragma once
#include "parser.h"

using namespace parser;
typedef unsigned char RGB[3];

typedef Vec3D<double> Point;

class IntersectionPoint
{
public:
    Point point;
    const Triangle *triangle;
    const Sphere *sphere;
    double distance;
    bool isSphere;
};