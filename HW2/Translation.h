#ifndef __TRANSLATION_H__
#define __TRANSLATION_H__

#include "Matrix4.h"
#include "Transformation.h"

class Translation : public Transformation
{
public:
    int translationId;
    double tx, ty, tz;

    Translation();
    Translation(int translationId, double tx, double ty, double tz);
    friend std::ostream &operator<<(std::ostream &os, const Translation &t);

    /***************************************************/
    void findTranslationMatrix();
};

#endif