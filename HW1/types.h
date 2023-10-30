#pragma once
#include "parser.h"
#include <limits>

using namespace parser;
typedef unsigned char RGB[3];

typedef Vec3D<double> Point;

constexpr double EPSILON = 3*std::numeric_limits<double>::epsilon();

class IntersectionPoint
{
public:
    Point point;
    const Triangle *triangle;
    const Sphere *sphere;
    double distance;
    bool isSphere;
};