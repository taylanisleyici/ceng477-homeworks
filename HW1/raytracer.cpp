#include <iostream>
#include "parser.h"
#include "ppm.h"
#include "types.h"
#include "Vec3D.h"
#include "Ray3D.h"
#include "helpers.h"

using namespace std;


typedef unsigned char RGB[3];


int main(int argc, char* argv[])
{
    // Sample usage for reading an XML scene file
    parser::Scene scene;

    scene.loadFromXml("./inputs/simple.xml");


    int light_count = scene.point_lights.size();

    for(int i = 0; i < scene.cameras.size(); i++)
    {
        Camera cam = scene.cameras[i];
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