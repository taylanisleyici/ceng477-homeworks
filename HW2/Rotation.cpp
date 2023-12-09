#include <iomanip>
#include <iostream>
#include "Rotation.h"
#include "math.h"

using namespace std;

Rotation::Rotation()
{
    // this->rotationId = -1;
    // this->angle = 0;
    // this->ux = 0;
    // this->uy = 0;
    // this->uz = 0;
    Rotation(-1, 0, 0, 0, 0);
}

Rotation::Rotation(int rotationId, double angle, double x, double y, double z)
{
    this->rotationId = rotationId;
    this->angle = angle;
    this->u.x = x;
    this->u.y = y;
    this->u.z = z;
    this->homogenous = this->findRotationMatrix();
}

Matrix4 Rotation::findRotationMatrix()
{
    Vec3 unitU = u.unit();
    double minOfU = min(unitU.x, min(unitU.y, unitU.z));

    Vec3 v = unitU;
    if (minOfU == unitU.x)
    {
        v.x = 0;
        swap(v.y, v.z);
        v.y = -v.y;
    }
    else if (minOfU == unitU.y)
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
    v = v.unit();
    w = w.unit();
    Matrix4 M;
    M[0][0] = unitU.x;
    M[0][1] = unitU.y;
    M[0][2] = unitU.z;
    M[0][3] = 0;
    M[1][0] = v.x;
    M[1][1] = v.y;
    M[1][2] = v.z;
    M[1][3] = 0;
    M[2][0] = w.x;
    M[2][1] = w.y;
    M[2][2] = w.z;
    M[2][3] = 0;
    M[3][3] = 1;

    Matrix4 R;
    R[0][0] = 1;
    R[1][1] = cos(angle);
    R[1][2] = -sin(angle);
    R[2][1] = sin(angle);
    R[2][2] = cos(angle);
    R[3][3] = 1;

    return M.transpose() * R * M;
}

std::ostream &operator<<(std::ostream &os, const Rotation &r)
{
    os << std::fixed << std::setprecision(3) << "Rotation " << r.rotationId << " => [angle=" << r.angle << ", " << r.u.x << ", " << r.u.y << ", " << r.u.z << "]";
    return os;
}