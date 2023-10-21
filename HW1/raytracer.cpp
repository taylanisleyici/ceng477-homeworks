#include <iostream>
#include "parser.h"
#include "ppm.h"
#include "types.h"
#include "Vec3D.h"
#include "Ray3D.h"
#include "helpers.h"

using namespace std;


typedef unsigned char RGB[3];

Ray3D computeRay(Vec3D<double> e, int i, int j, float distance, Vec3D<double> u, Vec3D<double> v, Vec3D<double> w, Vec4D plane, int width, int height)
{
    Vec3D<double> m = e + (opposite(w) * distance);
    Vec3D<double> q = m + (u * plane.x) + (v * plane.w);
    double s_u = (i + 0.5) * (plane.y - plane.x) / width;
    double s_v = (j + 0.5) * (plane.w - plane.z) / height;
    Vec3D<double> s = q + (s_u * u) - (s_v * v);

    return generateRay(e, (s-e));
}


int main(int argc, char* argv[])
{
    // Sample usage for reading an XML scene file
    parser::Scene scene;

    scene.loadFromXml("./inputs/simple.xml");


    int light_count = scene.point_lights.size();

    for(int i = 0; i < scene.cameras.size(); i++)
    {
        Camera cam = cam;
        Vec3D<double> v = cam.up;
        Vec3D<double> w = opposite(cam.gaze);
        Vec3D<double> u = crossProduct(v, w);

        int height = cam.image_height;
        int width = cam.image_width;

        for(int j = 0; j < height; j++)
        {
            for(int k = 0; k < width; k++)
            {
                Ray3D ray = computeRay(cam.position, j, k, cam.near_distance, u, v, w, cam.near_plane, width, height);

                // Check for intersection with each triangle
            }
        }
        

            
    }


    return 0;
}