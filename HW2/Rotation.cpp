#include <iomanip>
#include <iostream>
#include "Rotation.h"
#include "math.h"
#include "Translation.h"
#include "Helpers.h"

using namespace std;

Rotation::Rotation()
{
    this->rotationId = -1;
    this->angle = 0;
    this->u.x = 0;
    this->u.y = 0;
    this->u.z = 0;
}

Rotation::Rotation(int rotationId, double angle, double x, double y, double z)
{
    this->rotationId = rotationId;
    this->angle = angle;
    this->u.x = x;
    this->u.y = y;
    this->u.z = z;
    this->findRotationMatrix();
}

void Rotation::findRotationMatrix()
{
    Vec3 unitU = u.unit();
    double minOfU = min(abs(unitU.x), min(abs(unitU.y), abs(unitU.z)));

    Vec3 v = unitU;
    if (minOfU == abs(unitU.x))
    {
        v.x = 0;
        swap(v.y, v.z);
        v.y = -v.y;
    }
    else if (minOfU == abs(unitU.y))
    {
        v.y = 0;
        swap(v.x, v.z);
        v.x = -v.x;
    }
    else
    {
        v.z = 0;
        swap(v.x, v.y);
        v.x = -v.x;
    }

    Vec3 w = unitU * v;
    Matrix4 m;
    m[0][0] = u.x;
    m[0][1] = u.y;
    m[0][2] = u.z;
    m[0][3] = 0;
    m[1][0] = v.x;
    m[1][1] = v.y;
    m[1][2] = v.z;
    m[2][0] = w.x;
    m[2][1] = w.y;
    m[2][2] = w.z;
    m[3][3] = 1;
    Matrix4 tM(m);

    Matrix4 m2 = m.transpose();


    double rx[4][4] = {{1, 0, 0, 0}, {0, cos(this->angle * M_PI / 180), -sin(this->angle * M_PI / 180), 0}, {0, sin(this->angle * M_PI / 180), cos(this->angle * M_PI / 180), 0}, {0, 0, 0, 1}};
    Matrix4 TRx(rx);
    
    this->homogenous = m2 * TRx * tM;
    
    /***/
    // this->homogenous = getIdentityMatrix();
}

std::ostream &operator<<(std::ostream &os, const Rotation &r)
{
    os << std::fixed << std::setprecision(3) << "Rotation " << r.rotationId << " => [angle=" << r.angle << ", " << r.u.x << ", " << r.u.y << ", " << r.u.z << "]";
    return os;
}