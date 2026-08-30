#pragma once
#include "Color.h"

class Material{
public:
    Color ka,kd,ks;
    float shininess;
    Material();
    Material(const Color&, const Color&, const Color&,float);
};
