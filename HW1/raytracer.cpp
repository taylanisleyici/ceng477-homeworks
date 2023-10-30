#include <iostream>
#include "parser.h"
#include "ppm.h"
#include "types.h"
#include "Vec3D.h"
#include "Ray3D.h"
#include "helpers.h"
#include "BVH.h"

using namespace std;
using namespace parser;

// Swap two BVHLeaf pointers
void swap(BVHLeaf*& a, BVHLeaf*& b) {
    BVHLeaf* temp = a;
    a = b;
    b = temp;
}

// Partition the vector based on maxVertex.y
// int partition(std::vector<BVHLeaf*>& arr, int low, int high) {
//     float pivot = arr[high]->maxVertex.y;
//     int i = (low - 1);

//     for (int j = low; j <= high - 1; j++) {
//         if (arr[j]->maxVertex.y < pivot) {
//             i++;
//             // swap(arr[i], arr[j]);
//             BVHLeaf *temp = arr[i];
//             arr[i] = arr[j];
//             arr[j] = temp;
//         }
//     }

//     BVHLeaf *temp = arr[i+1];
//     arr[i+1] = arr[high];
//     arr[high] = temp;
//     return (i + 1);
// }

// // Quicksort function
// void quicksort(std::vector<BVHLeaf*>& arr, int low, int high) {
//     if (low < high) {
//         int pi = partition(arr, low, high);

//         quicksort(arr, low, pi - 1);
//         quicksort(arr, pi + 1, high);
//     }
// }

int partition(std::vector<BVHLeaf*>& arr, int low, int high) {
    float pivot = arr[high]->maxVertex.y;
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j]->maxVertex.y < pivot) {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return (i + 1);
}

void quicksort(std::vector<BVHLeaf*>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);
    }
}

Vec3D<double> shading(const Scene &scene, const Camera &camera, const IntersectionPoint &nearestIntersection, const Material &nearestMaterial)
{
    // COLOR BIMBIM BAMBAM
    Vec3D<double> color;
    color.x = 0;
    color.y = 0;
    color.z = 0;
    for (size_t i = 0; i < scene.point_lights.size(); i++)
    {
        // check shadow status !
        // this is common between diffuse and specular

        PointLight light = scene.point_lights[i];
        Vec3D<double> temp = light.position - nearestIntersection.point;
        double distance = magnitude(temp) - scene.shadow_ray_epsilon;
        Ray3D shadowRay = generateRay(light.position, nearestIntersection.point);

        IntersectionPoint shadowIntersection = intersectRay(shadowRay, scene);
        if (shadowIntersection.distance < distance)
        {
            // Shadow Logic ??
            continue;
        }

        // diffuse

        Vec3D<double> diffuse = nearestMaterial.diffuse;
        Vec3D<double> intensity = light.intensity;

        Vec3D<double> normal;

        if (nearestIntersection.isSphere)
        {
            normal = sphereNormal(nearestIntersection.point, scene.vertex_data[nearestIntersection.sphere->center_vertex_id - 1]);
        }
        else
        {
            normal = nearestIntersection.triangle->normal;
        }

        double cos_theta = dotProduct(unitVector(temp), normal) < 0 ? 0 : dotProduct(unitVector(temp), normal);

        color.x += diffuse.x * intensity.x * cos_theta / (distance * distance);
        color.y += diffuse.y * intensity.y * cos_theta / (distance * distance);
        color.z += diffuse.z * intensity.z * cos_theta / (distance * distance);

        // specular logic

        Vec3D<double> w_i = unitVector(light.position - nearestIntersection.point);
        Vec3D<double> w_o = unitVector(camera.position - nearestIntersection.point);

        Vec3D<double> h = (w_i + w_o) / magnitude(w_i + w_o);
        double cos_alpha = dotProduct(normal, h) < 0 ? 0 : dotProduct(normal, h);

        Vec3D<double> specular = nearestMaterial.specular;
        double phong = nearestMaterial.phong_exponent;

        color.x += specular.x * intensity.x * pow(cos_alpha, phong) / (distance * distance);
        color.y += specular.y * intensity.y * pow(cos_alpha, phong) / (distance * distance);
        color.z += specular.z * intensity.z * pow(cos_alpha, phong) / (distance * distance);
    }
    return color;
}

Vec3D<unsigned char> calculatePixelOfRay(const Ray3D &ray, const Scene &scene, const Camera &camera, BVHNode *root)
{
    IntersectionPoint nearestIntersection = root->intersect(ray, scene);
    // IntersectionPoint nearestIntersection = intersectRay(ray, scene);
    if (nearestIntersection.distance == numeric_limits<double>::max())
    {
        return Vec3D<unsigned char>((unsigned char) scene.background_color.x, (unsigned char) scene.background_color.y, (unsigned char) scene.background_color.z);
    }
    // TODO
    // TEST
    Material nearestMaterial = nearestIntersection.isSphere ? scene.materials[nearestIntersection.sphere->material_id - 1] : scene.materials[nearestIntersection.triangle->material_id - 1];
    double R, G, B;

    Vec3D<double> colorDouble = shading(scene, camera, nearestIntersection, nearestMaterial);

    R = ((scene.ambient_light.x * nearestMaterial.ambient.x + colorDouble.x)) + 0.5;
    G = ((scene.ambient_light.y * nearestMaterial.ambient.y + colorDouble.y)) + 0.5;
    B = ((scene.ambient_light.z * nearestMaterial.ambient.z + colorDouble.z)) + 0.5;

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

    return Vec3D<unsigned char>(R, G, B);
}

void renderImageFromCamera(const Camera &camera, const Scene &scene, BVHNode *root)
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
            if (k == 400 && j == 400)
            {
                cout << "PAT" << endl;
            } 
            Ray3D ray = computeRay(camera.position, k, j, camera.near_distance, u, v, w, camera.near_plane, width, height);
            auto pixelValues = calculatePixelOfRay(ray, scene, camera, root);
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

    // cout << scene.triangles.size() << endl;
    // cout << scene.spheres.size() << endl;

    cout << leafs.size() << endl;
    quicksort(leafs, 0, leafs.size() - 1);


    BVHNode *root = buildTree(leafs, 0, leafs.size() - 1);

    for (int i = 0; i < scene.cameras.size(); i++)
    {
        renderImageFromCamera(scene.cameras[i], scene, root);
    }

    return 0;
}