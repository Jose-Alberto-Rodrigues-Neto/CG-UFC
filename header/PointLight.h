#pragma once
#include "Vector3.h"
#include "Color.h"

class PointLight{
    Vector3 position_;
    Color   intensity_;
public:
    PointLight();
    PointLight(const Vector3&,const Color&);
    const Vector3& position ()const{return position_;}
    const Color&   intensity()const{return intensity_;}
};
