#include <iomanip>
#include <iostream>
#include "Scaling.h"

Scaling::Scaling() {
    // this->scalingId = -1;
    // this->sx = 0;
    // this->sy = 0;
    // this->sz = 0;
    Scaling(-1,0,0,0);
}

Scaling::Scaling(int scalingId, double sx, double sy, double sz)
{
    this->scalingId = scalingId;
    this->sx = sx;
    this->sy = sy;
    this->sz = sz;
    this->findScalingMatrix();
}

std::ostream &operator<<(std::ostream &os, const Scaling &s)
{
    os << std::fixed << std::setprecision(3) << "Scaling " << s.scalingId << " => [" << s.sx << ", " << s.sy << ", " << s.sz << "]";

    return os;
}

void Scaling::findScalingMatrix()
{
    this->homogenous[0][0] = sx;
    this->homogenous[1][1] = sy;
    this->homogenous[2][2] = sz;
    this->homogenous[3][3] = 1;
}