#pragma once
#include "Vector3.h"

class Ray{
public:
    Vector3 origin,direction;
    Ray();
    Ray(const Vector3&,const Vector3&);
    Vector3 at(float)const;
};
