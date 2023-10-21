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
IntersectionPoint intersectRay(const Ray3D &ray, const Scene &scene);
IntersectionPoint triangleArrayIntersectionHelper(const Ray3D &ray, const std::vector<Triangle> &triangles, const Scene &scene);
double determinant(std::vector<Vec3D<double>> matrix);
Vec3D<double> cramer(const std::vector<Vec3D<double>> &a, const Vec3D<double> &b);
Ray3D computeRay(Vec3D<double> e, int i, int j, float distance, Vec3D<double> u, Vec3D<double> v, Vec3D<double> w, Vec4D plane, int width, int height);
IntersectionPoint rayTriangleIntersection(const Ray3D &ray, const Triangle &triangle, const Scene &scene);
IntersectionPoint raySphereIntersection(const Ray3D &ray, const Sphere &sphere, const Scene &scene);


#endif