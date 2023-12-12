#ifndef __VEC4_H__
#define __VEC4_H__
#define NO_COLOR -1

class Vec4
{
public:
    double x, y, z, t;
    int colorId;

    Vec4();
    Vec4(double x, double y, double z, double t);
    Vec4(double x, double y, double z, double t, int colorId);
    Vec4(const Vec4 &other);

    Vec4 operator*(const Vec4 &other);
    Vec4 operator+(const Vec4 &other);
    Vec4 operator-(const Vec4 &other);
    Vec4 operator-();
    bool operator==(const Vec4 &other);
    bool operator!=(const Vec4 &other);
    Vec4 unit();
    double getNthComponent(int n);
    double magnitude();
    void toHomogenous();

    friend std::ostream &operator<<(std::ostream &os, const Vec4 &v);
};



#endif