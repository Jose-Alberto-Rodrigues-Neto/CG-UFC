#include "PointLight.h"

PointLight::PointLight():position_(0,5,5),intensity_(1,1,1){}
PointLight::PointLight(const Vector3&p,const Color&i):position_(p),intensity_(i){}
