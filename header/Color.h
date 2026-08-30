#pragma once
#include <algorithm>
class Color{
public:
    float r,g,b;
    Color();
    Color(float,float,float);
    Color clamped()const;
    Color  operator+ (const Color&)const;
    Color  operator* (float)const;
    Color  operator* (const Color&)const;
    Color& operator+=(const Color&);
};
