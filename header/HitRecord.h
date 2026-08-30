#pragma once
#include "Vector3.h"
#include "Material.h"

class Object;
class HitRecord{
public:
    float   t=0;
    Vector3 point,normal;
    const Material* material = nullptr;
    const Object* object=nullptr;
};
