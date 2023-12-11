#ifndef __SCALING_H__
#define __SCALING_H__

#include "Transformation.h"
#include "Matrix4.h"

class Scaling : public Transformation
{
public:
    int scalingId;
    double sx, sy, sz;

    Scaling();
    Scaling(int scalingId, double sx, double sy, double sz);
    friend std::ostream &operator<<(std::ostream &os, const Scaling &s);

    void findScalingMatrix();
};

#endif