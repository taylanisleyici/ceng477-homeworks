#include <iomanip>
#include <iostream>
#include "Matrix4.h"

Matrix4::Matrix4()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            this->values[i][j] = 0;
        }
    }
}

Matrix4::Matrix4(double values[4][4])
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            this->values[i][j] = values[i][j];
        }
    }
}

Matrix4::Matrix4(const Matrix4 &other)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            this->values[i][j] = other.values[i][j];
        }
    }
}

Matrix4 Matrix4::operator*(const Matrix4 &other)
{
    /*
     * If we will use this function too often, we should consider replacing it
     * with a more memory efficient version.
     */
    Matrix4 result;
    for (int i = 0; i < 4; i++)
    {
        for (int k = 0; k < 4; k++)
        {
            for (int j = 0; j < 4; j++)
            {
                result.values[i][k] += this->values[i][j] * other.values[j][k];
            }
        }
    }
    return result;
}

void Matrix4::multiplyWith(const Matrix4 &other)
{
    *this = *this * other;
}

double *Matrix4::operator[](int index)
{
    return values[index];
}

std::ostream &operator<<(std::ostream &os, const Matrix4 &m)
{

    os << std::fixed << std::setprecision(6) << "|" << m.values[0][0] << "|" << m.values[0][1] << "|" << m.values[0][2] << "|" << m.values[0][3] << "|"
       << std::endl
       << "|" << m.values[1][0] << "|" << m.values[1][1] << "|" << m.values[1][2] << "|" << m.values[1][3] << "|"
       << std::endl
       << "|" << m.values[2][0] << "|" << m.values[2][1] << "|" << m.values[2][2] << "|" << m.values[2][3] << "|"
       << std::endl
       << "|" << m.values[3][0] << "|" << m.values[3][1] << "|" << m.values[3][2] << "|" << m.values[3][3] << "|";

    return os;
}

Matrix4 Matrix4::transpose()
{
    Matrix4 ret;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            ret[i][j] = (*this)[j][i];
        }
    }
    return ret;
}

Vec4 Matrix4::operator*(const Vec4 &vec)
{
    Vec4 result;
    result.x = this->values[0][0] * vec.x + this->values[0][1] * vec.y + this->values[0][2] * vec.z + this->values[0][3] * vec.t;
    result.y = this->values[1][0] * vec.x + this->values[1][1] * vec.y + this->values[1][2] * vec.z + this->values[1][3] * vec.t;
    result.z = this->values[2][0] * vec.x + this->values[2][1] * vec.y + this->values[2][2] * vec.z + this->values[2][3] * vec.t;
    result.t = this->values[3][0] * vec.x + this->values[3][1] * vec.y + this->values[3][2] * vec.z + this->values[3][3] * vec.t;
    result.colorId = vec.colorId;
    return result;
}
