#include "Vec3D.h"

template <typename T>
Vec3D<T>::Vec3D()
{
    this->x = this->y = this->z = 0;
}

template <typename T>
Vec3D<T>::Vec3D(T x, T y, T z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

template <typename T>
double magnitude(const Vec3D<T> &a)
{
    return std::sqrt(dotProduct(a, a));
}

template <typename T>
Vec3D<double> unitVector(const Vec3D<T> &a)
{
    double mag = magnitude(a);
    return a/mag;
}

template <typename T>
Vec3D<double> opposite(const Vec3D<T> &a)
{
    return Vec3D<double>((0 - a.x), (0 - a.y), (0 - a.z));
}

template class Vec3D<double>;
template class Vec3D<long>;
template class Vec3D<unsigned char>;
template Vec3D<double> unitVector(const Vec3D<long> &a);
template Vec3D<double> unitVector(const Vec3D<double> &a);
template double magnitude(const Vec3D<double> &a);
template double magnitude(const Vec3D<long> &a);
template Vec3D<double> opposite(const Vec3D<double> &a);

