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



    // The code below creates a test pattern and writes
    // it to a PPM file to demonstrate the usage of the
    // ppm_write function.
    //
    // Normally, you would be running your ray tracing
    // code here to produce the desired image.
    // Vec3D<double> a(1.2,2.3,3.4);
    // Vec3D<long> b(4,5,6);
    // auto c = a * 3;
    // auto c2 = 3 * a;
    // auto f = b*3.1;
    // auto f2 = 3.1*b;
    // auto f3 = b/3;
    // auto d = dotProduct(a, b);
    // auto e = crossProduct(a, b);
    Vec3D<double> a(1,-1,0);
    Vec3D<double> b(1,1,0);
    Vec3D<double> c(0,0,-0.5);
    Vec3D<double> d(1,1,1);
    vector<Vec3D<double>> det = {a,b,c};
    Vec3D<double> result = cramer(det, d);
    return 0;
}