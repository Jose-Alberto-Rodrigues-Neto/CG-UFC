#pragma once
#include "Object.h"

class Sphere: public Object{
    Vector3 center_;
    float   radius_;
    Material material_;
public:
    Sphere(const Vector3&,float,const Material&);
    bool intersect(const Ray&,float,float,HitRecord&)const override;
};
