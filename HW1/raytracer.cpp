#include <iostream>
#include "parser.h"
#include "ppm.h"
#include "types.h"
#include "Vec3D.h"
#include "Ray3D.h"
#include "helpers.h"

using namespace std;
using namespace parser;

Vec3D<unsigned char> calculatePixelOfRay(const Ray3D &ray, const Scene &scene)
{
    IntersectionPoint nearestIntersection = intersectRay(ray, scene);
}

void renderImageFromCamera(const Camera &camera, const Scene &scene)
{
    Vec3D<double> v = camera.up;
    Vec3D<double> w = opposite(camera.gaze);
    Vec3D<double> u = crossProduct(v, w);

    int height = camera.image_height;
    int width = camera.image_width;

    unsigned char image[height * width * 3];
    for (int j = 0; j < height; j++)
    {
        for (int k = 0; k < width; k++)
        {
            Ray3D ray = computeRay(camera.position, j, k, camera.near_distance, u, v, w, camera.near_plane, width, height);

        }
    }
}

int main(int argc, char *argv[])
{
    // Sample usage for reading an XML scene file
    parser::Scene scene;

    scene.loadFromXml("./inputs/simple.xml");

    int light_count = scene.point_lights.size();

    for (int i = 0; i < scene.cameras.size(); i++)
    {
        renderImageFromCamera(scene.cameras[i], scene);
    }

    return 0;
}