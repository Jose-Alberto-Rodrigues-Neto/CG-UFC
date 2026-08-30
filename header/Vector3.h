#pragma once
#include <cmath>

class Vector3 {
public:
    float   x, y, z;
    Vector3();
    Vector3(float x,float y,float z);
    float   length() const;
    Vector3 normalized() const;
    float   dot(const Vector3&)   const;
    Vector3 cross(const Vector3&) const;
    Vector3 operator+(const Vector3&) const;
    Vector3 operator-(const Vector3&) const;
    Vector3 operator-() const;
    Vector3 operator*(float) const;
    Vector3 operator/(float) const;
};

Vector3 operator*(float,const Vector3&);
