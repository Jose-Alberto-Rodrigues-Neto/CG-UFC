#include "Sphere.h"
#include <cmath>

Sphere::Sphere(const Vector3&c,float r,const Material&m):center_(c),radius_(r),material_(m){}

bool Sphere::intersect(const Ray&ray,float tMin,float tMax,HitRecord&hit)const{
 // STUDENT TODO 1: intersecao raio-esfera.
    Vector3 oc=ray.origin-center_;
    float   a=ray.direction.dot(ray.direction);
    float   hb=oc.dot(ray.direction);
    float   c=oc.dot(oc)-radius_*radius_;
    float   disc=hb*hb-a*c;
    if(disc<0)return false;
    float s=std::sqrt(disc);
    float t=(-hb-s)/a;
    if (t<tMin||t>tMax){
        t = (-hb+s)/a;
        if (t<tMin||t>tMax) return false;
    }
    hit.t        = t;
    hit.point    = ray.at(t);
    hit.normal   = (hit.point-center_)/radius_;
    hit.material = &material_;
    hit.object   = this;
    return true;
}
