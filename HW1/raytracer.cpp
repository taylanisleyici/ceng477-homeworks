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
    float pivot = arr[high]->maxVertex.x;
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j]->maxVertex.x < pivot) {
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
    Material nearestMaterial = nearestIntersection.isSphere ? scene.materials[nearestIntersection.sphere->material_id - 1] : scene.materials[nearestIntersection.triangle->material_id - 1]; // Change it with if
    double R = 0,G = 0,B = 0;

    // mirror logic
    if(nearestMaterial.is_mirror)
    {
        Vec3D<double> normal = findNormal(nearestIntersection, scene);
        Vec3D<double> w_o = unitVector(camera.position - nearestIntersection.point);
        Vec3D<double> w_r = -(w_o) + (2 * normal * dotProduct(normal, w_o));
        Ray3D mirrorRay = Ray3D(nearestIntersection.point, w_r);

        Vec3D<double> mirrorColor = mirrorObject(scene, camera, mirrorRay, scene.max_recursion_depth, root);
        R += mirrorColor.x * nearestMaterial.mirror.x;
        G += mirrorColor.y * nearestMaterial.mirror.y;
        B += mirrorColor.z * nearestMaterial.mirror.z;
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
            if (k == 575 && j == 235)
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