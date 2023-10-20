#ifndef VEC3D_H
#define VEC3D_H

#include <cmath>

template <typename T>
class Vec3D
{
public:
    T x, y, z;
    Vec3D();
    Vec3D(T x, T y, T z);
    
    template <typename U>
    friend Vec3D<decltype(T{} + U{})> operator+(const Vec3D<T> &lhs, const Vec3D<U> &rhs)
    {
        return Vec3D<decltype(T{} + U{})>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
    }
    
    template <typename U>
    friend Vec3D<decltype(T{} + U{})> operator-(const Vec3D<T> &lhs, const Vec3D<U> &rhs)
    {
        return Vec3D<decltype(T{} + U{})>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
    }

    template <typename U>
    friend Vec3D<decltype(T{} + U{})> operator*(const U &lhs, const Vec3D<T> &rhs)
    {
        return Vec3D<decltype(T{} + U{})>(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
    }
    
    template <typename U>
    friend Vec3D<decltype(T{} + U{})> operator*(const Vec3D<T> &lhs, const U &rhs)
    {
        return Vec3D<decltype(T{} + U{})>(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
    }

    template <typename U>
    friend Vec3D<double> operator/(const Vec3D<T> &lhs, const U &rhs)
    {
        return Vec3D<double>((double)lhs.x / rhs, (double)lhs.y / rhs, (double)lhs.z / rhs);
    }
};

template <typename T, typename U>
auto dotProduct(const Vec3D<T> &lhs, const Vec3D<U> &rhs) -> decltype(T{} + U{})
{
    return rhs.x * lhs.x + rhs.y * lhs.y + rhs.z * lhs.z;
}

template <typename T, typename U>
auto crossProduct(const Vec3D<T> &a, const Vec3D<U> &b) -> Vec3D<decltype(a.x * b.x)>
{
    Vec3D<decltype(a.x * b.x)> result;
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
    return result;
}

template <typename T>
Vec3D<double> unitVector(const Vec3D<T> &a);

template <typename T>
double magnitude(const Vec3D<T> &a);

#endif