#include "Color.h"

Color::Color():r(0),g(0),b(0){}
Color::Color(float R,float G,float B):r(R),g(G),b(B){}
Color Color::clamped()const{
    return{std::clamp(r,0.f,1.f),std::clamp(g,0.f,1.f),std::clamp(b,0.f,1.f)};
}
Color  Color::operator+ (const Color&o)const{return{r+o.r,g+o.g,b+o.b};}
Color  Color::operator* (float s)      const{return{r*s,g*s,b*s};}
Color  Color::operator* (const Color&o)const{return{r*o.r,g*o.g,b*o.b};}
Color& Color::operator+=(const Color&o){
    r+=o.r;
    g+=o.g;
    b+=o.b;
    return *this;}
