#include "Vector3.h"

Vector3::Vector3():x(0),y(0),z(0){}

Vector3::Vector3(float X,float Y,float Z):x(X),y(Y),z(Z){}

float Vector3::length() const{return std::sqrt(x*x+y*y+z*z);
}

Vector3 Vector3::normalized() const{
    float  l = length();
    return l<1e-8f?Vector3():*this/l;
}

float Vector3::dot(const Vector3&o) const{
    return x*o.x+y*o.y+z*o.z;
}

Vector3 Vector3::cross(const Vector3&o) const{
    return {y*o.z-z*o.y,z*o.x-x*o.z,x*o.y-y*o.x};
}

Vector3 Vector3::operator+(const Vector3&o)const{
    return{x+o.x,y+o.y,z+o.z};
}

Vector3 Vector3::operator-(const Vector3&o)const{
    return{x-o.x,y-o.y,z-o.z};
}

Vector3 Vector3::operator-()const{
    return{-x,-y,-z};
}

Vector3 Vector3::operator*(float s)const{
    return{x*s,y*s,z*s};
}

Vector3 Vector3::operator/(float s)const{
    return{x/s,y/s,z/s};
}

Vector3 operator*(float s,const Vector3&v){
    return v*s;
}
