#include "Ray.h"
Ray::Ray():origin(),direction(0,0,-1){}
Ray::Ray(const Vector3&o,const Vector3&d):origin(o),direction(d.normalized()){}
Vector3 Ray::at(float t)const {return origin+direction*t;}
