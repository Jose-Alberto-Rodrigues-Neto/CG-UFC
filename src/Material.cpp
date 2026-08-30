#include "Material.h"

Material::Material():ka(.1,.1,.1),kd(.7,.7,.7),ks(1,1,1),shininess(32){}
Material::Material(const Color&a,const Color&d,const Color&s,float n):ka(a),kd(d),ks(s),shininess(n){}
