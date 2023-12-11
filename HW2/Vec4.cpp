#include <iomanip>
#include <iostream>
#include "Vec4.h"

#include "math.h"

using namespace std;

Vec4::Vec4()
{
    this->x = 0.0;
    this->y = 0.0;
    this->z = 0.0;
    this->t = 0.0;
    this->colorId = NO_COLOR;
}

Vec4::Vec4(double x, double y, double z, double t)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->t = t;
    this->colorId = NO_COLOR;
}

Vec4::Vec4(double x, double y, double z, double t, int colorId)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->t = t;
    this->colorId = colorId;
}

Vec4::Vec4(const Vec4 &other)
{
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
    this->t = other.t;
    this->colorId = other.colorId;
}

double Vec4::getNthComponent(int n)
{
    switch (n)
    {
    case 0:
        return this->x;

    case 1:
        return this->y;

    case 2:
        return this->z;

    case 3:
    default:
        return this->t;
    }
}

std::ostream &operator<<(std::ostream &os, const Vec4 &v)
{
    os << std::fixed << std::setprecision(6) << "[" << v.x << ", " << v.y << ", " << v.z << ", " << v.t << "]";
    return os;
}

Vec4 Vec4::operator*(const Vec4 &other)
{
    /*
     * Vec4 doesn't normally have property of cross product, but since the
     * purpose of Vec4 is to represent homogenous coordinates, we will go
     * ahead and implement it as it's a 3d vector.
     */
    return Vec4{
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x,
        1};
}

Vec4 Vec4::operator+(const Vec4 &other)
{
    return Vec4{
        this->x + other.x,
        this->y + other.y,
        this->z + other.z,
        1};
}

Vec4 Vec4::operator-(const Vec4 &other)
{
    /*
    * We should be careful while using these operators. We are always
    * assuming that the homogenous coordinate is 1.
    */
    return Vec4{
        this->x - other.x,
        this->y - other.y,
        this->z - other.z,
        1};
}

void Vec4::toHomogenous()
{
    this->x /= this->t;
    this->y /= this->t;
    this->z /= this->t;
    this->t = 1;
}

Vec4 Vec4::operator-()
{
    return Vec4{
        -this->x,
        -this->y,
        -this->z,
        this->t, this->colorId};
}

double Vec4::magnitude()
{
    return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
}

Vec4 Vec4::unit()
{
    double mag = this->magnitude();
    return Vec4{
        this->x / mag,
        this->y / mag,
        this->z / mag,
        this->t, this->colorId};
}