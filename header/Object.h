#pragma once
#include "Ray.h"
#include "HitRecord.h"

class Object{
public:
    virtual ~Object() = default;
    virtual bool intersect(const Ray&, float, float, HitRecord&)const=0;
};
