#include <iostream>
#include "parser.h"
#include "ppm.h"
#include "types.h"
#include "Vec3D.h"
#include "Ray3D.h"
#include "helpers.h"

using namespace std;
using namespace parser;


Vec3D<double> diffuseShading(const Scene &scene, const Camera &camera, const IntersectionPoint &nearestIntersection, const Material &nearestMaterial)
{
    // DIFFUSE BIMBIM BAMBAM
    // for(size_t i = 0; i < scene.point_lights.size(); i++)
    // {
        // check shadow status !
        // this is common between diffuse and specular so avoid multiple 
    PointLight light = scene.point_lights[0];
    Vec3D<double> temp = light.position - nearestIntersection.point;
    double distance = magnitude(temp) - scene.shadow_ray_epsilon;
    Ray3D shadowRay = generateRay(light.position, nearestIntersection.point);

    IntersectionPoint shadowIntersection = intersectRay(shadowRay, scene);
    if(shadowIntersection.distance < distance)
    {
        // Shadow Logic ??
        Vec3D<double> shadow;
        shadow.x = 0;
        shadow.y = 0;
        shadow.z = 0;
        return shadow;
    }
    //diffuse


    Vec3D<double> diffuse = nearestMaterial.diffuse;
    Vec3D<double> intensity = light.intensity;
    
    Vec3D<double> normal;

    if(nearestIntersection.isSphere)
    {
        normal = sphereNormal(nearestIntersection.point, scene.vertex_data[nearestIntersection.sphere->center_vertex_id - 1]);
    }
    else
    {
        normal = nearestIntersection.triangle->normal;
    }
    
    double cos_theta = dotProduct(unitVector(temp), normal) < 0 ? 0 : dotProduct(unitVector(temp), normal);

    Vec3D<double> color;
    color.x = diffuse.x * intensity.x * cos_theta / (distance * distance);
    color.y = diffuse.y * intensity.y * cos_theta / (distance * distance);
    color.z = diffuse.z * intensity.z * cos_theta / (distance * distance);

    // specular logic

    //find the half vector h
    // find the angle between normal and h
    // do math
    // w_i is shadow ray opposite
    // w_o calculate it cam - point

    Vec3D<double> w_i = unitVector(light.position - nearestIntersection.point);
    Vec3D<double> w_o = unitVector(camera.position - nearestIntersection.point);

    Vec3D<double> h = (w_i + w_o) / magnitude(w_i + w_o);

    double cos_alpha = dotProduct(normal, h) < 0 ? 0 : dotProduct(normal, h);

    Vec3D<double> specular = nearestMaterial.specular;
    double phong = nearestMaterial.phong_exponent;

    color.x += specular.x * intensity.x * pow(cos_alpha, phong) / (distance * distance);
    color.y += specular.y * intensity.y * pow(cos_alpha, phong) / (distance * distance);
    color.z += specular.z * intensity.z * pow(cos_alpha, phong) / (distance * distance);

    return color;        
    // }
}

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
    double R,G,B;
        
    Vec3D<double> colorDouble = diffuseShading(scene, camera, nearestIntersection, nearestMaterial);
    
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
    
    
    // Vec3D<double> colorDouble =255 * nearestMaterial.diffuse;
    // R = (unsigned char) (colorDouble.x) + 0.5 - (colorDouble.x < 0.5);
    // G = (unsigned char) (colorDouble.x) + 0.5 - (colorDouble.x < 0.5); 
    // B = (unsigned char) (colorDouble.x) + 0.5 - (colorDouble.x < 0.5); 
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

    scene.loadFromXml("./inputs/simple_shading.xml");

    int light_count = scene.point_lights.size();

    for (int i = 0; i < scene.cameras.size(); i++)
    {
        renderImageFromCamera(scene.cameras[i], scene);
    }

    return 0;
}