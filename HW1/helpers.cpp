#include "helpers.h"

using namespace std;

Ray3D generateRay(Point startingPoint, Point targetPoint)
{
    Ray3D ray;
    ray.o = startingPoint;
    ray.d = unitVector(targetPoint - startingPoint);
    return ray;
}

IntersectionPoint intersectRay(const Ray3D &ray, const Scene &scene)
{
    IntersectionPoint returnPoint;
    IntersectionPoint currentPoint;
    returnPoint.distance = numeric_limits<double>::max();
    currentPoint = triangleArrayIntersectionHelper(ray, scene.triangles, scene);
    if (currentPoint.distance < returnPoint.distance) returnPoint = currentPoint;
    for (size_t i = 0; i < scene.meshes.size(); i++)
    {
        currentPoint = triangleArrayIntersectionHelper(ray, scene.meshes[i].faces, scene);
        if (currentPoint.distance < returnPoint.distance) 
        {
            returnPoint = currentPoint;
            returnPoint.isSphere = false;
        }
    }
    for (size_t i = 0; i < scene.spheres.size(); i++)
    {
        currentPoint = raySphereIntersection(ray, scene.spheres[i], scene);
        if (currentPoint.distance < returnPoint.distance)
        {
            returnPoint = currentPoint;
            returnPoint.isSphere = true;
        }
    }
    return returnPoint;
}

IntersectionPoint triangleArrayIntersectionHelper(const Ray3D &ray, const std::vector<Triangle> &triangles, const Scene &scene)
{
    IntersectionPoint returnPoint;
    IntersectionPoint currentPoint;
    returnPoint.distance = numeric_limits<double>::max();
    for (size_t i = 0; i < triangles.size(); i++)
    {
        currentPoint = rayTriangleIntersection(ray, triangles[i], scene);
        if (currentPoint.distance < returnPoint.distance)
        {
            returnPoint = currentPoint;
            returnPoint.isSphere = false;
        }
    }
    return returnPoint;
}

IntersectionPoint rayTriangleIntersection(const Ray3D &ray, const Triangle &triangle, const Scene &scene)
{
    // We should create a Ax = B equation
    // B is origin of ray - triangle's 3rd Vertex
    // A is triangle's 1st vertex - 3rd, 2nd - 3rd, - ray's d
    Vec3D<double> a,b,c;
    a = scene.vertex_data[triangle.indices.v0_id - 1];
    b = scene.vertex_data[triangle.indices.v1_id - 1];
    c = scene.vertex_data[triangle.indices.v2_id - 1];
    vector<Vec3D<double>> A = {a-b, a-c, ray.d};
    Vec3D<double> B = a - ray.o;

    Vec3D<double> alphaBetaT = cramer(A, B); // Alpha, Beta, T
    IntersectionPoint returnPoint;
    if (alphaBetaT.z == numeric_limits<double>::max() || alphaBetaT.z < 0 || alphaBetaT.x < 0 || alphaBetaT.x > 1 || alphaBetaT.y < 0 || alphaBetaT.y > 1 || alphaBetaT.x + alphaBetaT.y > 1)
    {
        returnPoint.distance = numeric_limits<double>::max();
        return returnPoint;
    }
    
    returnPoint.point = alphaBetaT.z * ray.d + ray.o;
    returnPoint.triangle = &triangle;
    returnPoint.distance = alphaBetaT.z;
    returnPoint.isSphere = false;
    return returnPoint;
}

IntersectionPoint raySphereIntersection(const Ray3D &ray, const Sphere &sphere, const Scene &scene)
{
    IntersectionPoint returnPoint;
    returnPoint.distance = numeric_limits<double>::max();
    Vec3D<double> center = scene.vertex_data[sphere.center_vertex_id - 1];
    double radius = sphere.radius;
    Vec3D<double> temp = ray.o - center;
    double a = dotProduct(ray.d, ray.d);
    double b = 2 * dotProduct(ray.d, temp);
    double c = dotProduct(temp, temp) - radius * radius;
    double delta = (b * b) - (4 * a * c);
    if (delta < 0)
    {
        returnPoint.distance = numeric_limits<double>::max();
        return returnPoint;
    }
    double t1 = (-b + sqrt(delta)) / (2 * a);
    double t2 = (-b - sqrt(delta)) / (2 * a);
    if (t1 < 0 && t2 < 0)
    {
        returnPoint.distance = numeric_limits<double>::max();
        return returnPoint;
    }
    if (t1 < 0)
    {
        returnPoint.distance = t2;
        returnPoint.point = ray.o + (t2 * ray.d);
        returnPoint.sphere = &sphere;
        returnPoint.isSphere = true;
        return returnPoint;
    }
    if (t2 < 0)
    {
        returnPoint.distance = t1;
        returnPoint.point = ray.o + (t1 * ray.d);
        returnPoint.sphere = &sphere;
        returnPoint.isSphere = true;
        return returnPoint;
    }
    if (t1 < t2)
    {
        returnPoint.distance = t1;
        returnPoint.point = ray.o + (t1 * ray.d);
        returnPoint.sphere = &sphere;
        returnPoint.isSphere = true;
        return returnPoint;
    }
    else
    {
        returnPoint.distance = t2;
        returnPoint.point = ray.o + (t2 * ray.d);
        returnPoint.sphere = &sphere;
        returnPoint.isSphere = true;
        return returnPoint;
    }
}

double determinant(vector<Vec3D<double>> matrix)
{
    if (matrix.size()!=3)
    {
        cout << "PAT" << endl;
        return 0;
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
        result.z = numeric_limits<double>::max();
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

Ray3D computeRay(Vec3D<double> cameraPosition, int i, int j, float distance, Vec3D<double> u, Vec3D<double> v, Vec3D<double> w, Vec4D plane, int width, int height)
{
    Vec3D<double> m = cameraPosition + (-w * distance);
    Vec3D<double> q = m + (u * plane.x) + (v * plane.w);
    double s_u = (i + 0.5) * (plane.y - plane.x) / width;
    double s_v = (j + 0.5) * (plane.w - plane.z) / height;
    Vec3D<double> s = q + (s_u * u) - (s_v * v);

    return generateRay(cameraPosition, s);
}