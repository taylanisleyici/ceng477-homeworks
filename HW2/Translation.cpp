#include <iomanip>
#include <iostream>
#include "Translation.h"

Translation::Translation()
{
    // this->translationId = -1;
    // this->tx = 0.0;
    // this->ty = 0.0;
    // this->tz = 0.0;
    Translation(-1, 0, 0, 0);
}

Translation::Translation(int translationId, double tx, double ty, double tz)
{
    this->translationId = translationId;
    this->tx = tx;
    this->ty = ty;
    this->tz = tz;
    findTranslationMatrix();
}


std::ostream &operator<<(std::ostream &os, const Translation &t)
{
    os << std::fixed << std::setprecision(3) << "Translation " << t.translationId << " => [" << t.tx << ", " << t.ty << ", " << t.tz << "]";
    return os;
}


void Translation::findTranslationMatrix()
{
    this->homogenous[0][0] = this->homogenous[1][1] = this->homogenous[2][2] = this->homogenous[3][3] = 1;
    this->homogenous[0][3] = tx;
    this->homogenous[1][3] = ty;
    this->homogenous[2][3] = tz;
}