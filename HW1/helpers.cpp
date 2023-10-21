#include "helpers.h"

using namespace std;

Ray3D generateRay(Point startingPoint, Point targetPoint)
{
    Ray3D ray;
    ray.o = startingPoint;
    ray.d = unitVector(targetPoint - startingPoint);
    return ray;
}

IntersectionPoint intersectRay(Ray3D ray, Scene *scene)
{
    IntersectionPoint returnPoint;
    double shortestDistance = numeric_limits<double>::max();
}

pair<Triangle, double> triangleVectorIntersectionHelper(Ray3D ray, const std::vector<Triangle> &triangles)
{
    return pair<Triangle, double>();
}

double determinant(vector<Vec3D<double>> matrix)
{
    if (matrix.size()!=3)
    {
        cout << "PAT" << endl;
    }

    double col1, col2, col3;
    col1 = matrix[0].x * (matrix[1].y * matrix[2].z - matrix[1].z * matrix[2].y);
    col2 = matrix[1].x * (matrix[0].y * matrix[2].z - matrix[2].y * matrix[0].z);
    col3 = matrix[2].x * (matrix[0].y * matrix[1].z - matrix[1].y * matrix[0].z);
    return col1 - col2 + col3;
}

Vec3D<double> cramer(const vector<Vec3D<double>> &a, const Vec3D<double> &b)//Ax = B
{
    
    Vec3D<double> result;
    double detOfA = determinant(a);
    if (detOfA == 0)
    {
        result.z = numeric_limits<long>::max();
        return result;
    }


    vector<Vec3D<double>> placeHolder(a);
    placeHolder[0] = b;
    result.x = determinant(placeHolder) / detOfA;
    placeHolder[0] = a[0];
    placeHolder[1] = b;
    result.y = determinant(placeHolder) / detOfA;
    placeHolder[1] = a[1];
    placeHolder[2] = b;
    result.z = determinant(placeHolder) / detOfA;
    return result;
}

Ray3D computeRay(Vec3D<double> e, int i, int j, float distance, Vec3D<double> u, Vec3D<double> v, Vec3D<double> w, Vec4D plane, int width, int height)
{
    Vec3D<double> m = e + (opposite(w) * distance);
    Vec3D<double> q = m + (u * plane.x) + (v * plane.w);
    double s_u = (i + 0.5) * (plane.y - plane.x) / width;
    double s_v = (j + 0.5) * (plane.w - plane.z) / height;
    Vec3D<double> s = q + (s_u * u) - (s_v * v);

    return generateRay(e, (s-e));
}