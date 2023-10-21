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
    if (nearestIntersection.distance == numeric_limits<double>::max())
    {

        //TODO
        //Type a long to unsigned char caster
        // return scene.background_color;
        return Vec3D<unsigned char>(0,0,0);
    }
    //TODO
    //TEST
    Material nearestMaterial = scene.materials[nearestIntersection.triangle->material_id - 1];
    Vec3D<double> colorDouble =255 * nearestMaterial.diffuse;
    unsigned char R,G,B;
    R = (unsigned char) (colorDouble.x) + 0.5 - (colorDouble.x < 0.5);
    G = (unsigned char) (colorDouble.x) + 0.5 - (colorDouble.x < 0.5); 
    B = (unsigned char) (colorDouble.x) + 0.5 - (colorDouble.x < 0.5); 
    return Vec3D<unsigned char>(R,G,B);
}

void renderImageFromCamera(const Camera &camera, const Scene &scene)
{
    Vec3D<double> v = camera.up;
    Vec3D<double> w = -camera.gaze;
    Vec3D<double> u = crossProduct(v, w);

    int height = camera.image_height;
    int width = camera.image_width;

    unsigned char image[height * width * 3];
    size_t i = 0;
    for (size_t j = 0; j < height; j++)
    {
        for (size_t k = 0; k < width; k++)
        {
            Ray3D ray = computeRay(camera.position, j, k, camera.near_distance, u, v, w, camera.near_plane, width, height);
            auto pixelValues = calculatePixelOfRay(ray, scene);
            image[i++] = pixelValues.x;
            image[i++] = pixelValues.y;
            image[i++] = pixelValues.z;
        }
    }
    write_ppm(camera.image_name.c_str(), image, width, height);
}

int main(int argc, char *argv[])
{
    parser::Scene scene;

    scene.loadFromXml("./inputs/simple.xml");

    int light_count = scene.point_lights.size();

    for (int i = 0; i < scene.cameras.size(); i++)
    {
        renderImageFromCamera(scene.cameras[i], scene);
    }

    return 0;
}