#include <iostream>
#include "parser.h"
#include "ppm.h"
#include "types.h"
#include "Vec3D.h"
#include "Ray3D.h"
#include "helpers.h"

using namespace std;
using namespace parser;


Vec3D<unsigned char> calculatePixelOfRay(const Ray3D &ray, const Scene &scene, const Camera &camera)
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
    Material nearestMaterial = nearestIntersection.isSphere ? scene.materials[nearestIntersection.sphere->material_id - 1] : scene.materials[nearestIntersection.triangle->material_id - 1];
    double R = 0,G = 0,B = 0;

    // mirror logic
    if(nearestMaterial.is_mirror)
    {
        Vec3D<double> normal = findNormal(nearestIntersection, scene);
        Vec3D<double> w_o = unitVector(camera.position - nearestIntersection.point);
        Vec3D<double> w_r = (-w_o) + (2 * normal * dotProduct(w_o, normal));

        Ray3D mirrorRay = Ray3D((nearestIntersection.point + (normal * scene.shadow_ray_epsilon)), w_r);

        Vec3D<double> mirrorColor = mirrorObject(scene, camera,nearestMaterial, mirrorRay, scene.max_recursion_depth);
        R += mirrorColor.x;
        G += mirrorColor.y;
        B += mirrorColor.z;
    }

    Vec3D<double> colorDouble = shading(scene, camera, nearestIntersection, nearestMaterial);
    
    R += colorDouble.x + 0.5;
    G += colorDouble.y + 0.5;
    B += colorDouble.z + 0.5;

    // clamp
    if (R > 255)
    {
        R = 255;
    }
    if (G > 255)
    {
        G = 255;
    }
    if (B > 255)
    {
        B = 255;
    }

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
            Ray3D ray = computeRay(camera.position, k, j, camera.near_distance, u, v, w, camera.near_plane, width, height);
            auto pixelValues = calculatePixelOfRay(ray, scene, camera);
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

    scene.loadFromXml("inputs/simple.xml");

    int light_count = scene.point_lights.size();

    for (int i = 0; i < scene.cameras.size(); i++)
    {
        renderImageFromCamera(scene.cameras[i], scene);
    }

    return 0;
}