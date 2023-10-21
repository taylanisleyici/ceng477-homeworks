#ifndef __HELPERS_H__
#define __HELPERS_H__

#include <vector>
#include <limits>
#include <utility>
#include <iostream>
#include "parser.h"
#include "ppm.h"
#include "types.h"
#include "Vec3D.h"
#include "Ray3D.h"

Ray3D generateRay(Point startingPoint, Point targetPoint);
IntersectionPoint intersectRay(Ray3D ray, Scene *scene);
std::pair<Triangle, double> triangleVectorIntersectionHelper(Ray3D ray, const std::vector<Triangle> &triangles);
double determinant(std::vector<Vec3D<double>> matrix);
Vec3D<double> cramer(const std::vector<Vec3D<double>> &a, const Vec3D<double> &b);
Ray3D computeRay(Vec3D<double> e, int i, int j, float distance, Vec3D<double> u, Vec3D<double> v, Vec3D<double> w, Vec4D plane, int width, int height);



#endif