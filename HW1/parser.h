#ifndef __HW1__PARSER__
#define __HW1__PARSER__

#include <string>
#include <vector>
#include "Vec3D.h"

namespace parser
{

    // Notice that all the structures are as simple as possible
    // so that you are not enforced to adopt any style or design.
    

    struct Vec4D
    {
        double x, y, z, w;
    };

    struct Camera
    {
        Vec3D<double> position;
        Vec3D<double> gaze;
        Vec3D<double> up;
        Vec4D near_plane;
        float near_distance;
        int image_width, image_height;
        std::string image_name;
    };

    struct PointLight
    {
        Vec3D<double> position;
        Vec3D<double> intensity;
    };

    struct Material
    {
        bool is_mirror;
        Vec3D<double> ambient;
        Vec3D<double> diffuse;
        Vec3D<double> specular;
        Vec3D<double> mirror;
        float phong_exponent;
    };

    struct Face
    {
        int v0_id;
        int v1_id;
        int v2_id;
    };

    struct Triangle
    {
        int material_id;
        Face indices;
    };

    struct Mesh
    {
        int material_id;
        std::vector<Triangle> faces;
    };

    struct Sphere
    {
        int material_id;
        int center_vertex_id;
        float radius;
    };

    struct Scene
    {
        // Data
        Vec3D<long> background_color;
        float shadow_ray_epsilon;
        int max_recursion_depth;
        std::vector<Camera> cameras;
        Vec3D<double> ambient_light;
        std::vector<PointLight> point_lights;
        std::vector<Material> materials;
        std::vector<Vec3D<double>> vertex_data;
        std::vector<Mesh> meshes;
        std::vector<Triangle> triangles;
        std::vector<Sphere> spheres;

        // Functions
        void loadFromXml(const std::string &filepath);
    };
}

#endif
