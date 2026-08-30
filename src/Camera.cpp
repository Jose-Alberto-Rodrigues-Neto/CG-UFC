#include "Camera.h"
#include <cmath>

Camera::Camera():eye_(0,0,0),lookAt_(0,0,-5),up_(0,1,0),fov_(45){updateBasis();}

Camera::Camera(const Vector3&e,const Vector3&l,const Vector3&u,float f):eye_(e),lookAt_(l),up_(u),fov_(f){updateBasis();}

void Camera::updateBasis(){
    w_ = (eye_-lookAt_).normalized();
    u_ = up_.cross(w_).normalized();
    v_ = w_.cross(u_).normalized();
}

Vector3 Camera::getEye()const{
    return eye_;
}

Ray Camera::generateRay(int px,int py,int width,int height)const{
 // STUDENT TODO 2: gerar raio primario para o centro do pixel.
    constexpr float PI = 3.14159265358979323846f;
    float  aspect      = float(width)/height;
    float  hh          = std::tan((fov_*PI/180.f)*.5f);
    float  hw          = aspect*hh;
    float  sx          = ((px+.5f)/width)*2.f - 1.f;
    float  sy          = 1.f - ((py+.5f)/height)*2.f;
    /*
    float  hj          = std::tan((fov_*PI/180.f)*.5f)*std::abs(eye_.z)*2.f;
    float  wj          = aspect*hj;
    float  deltax      = wj/width;
    float  deltay      = hj/height;
    float  cpx = -wj/2.f + deltax/2.f + px*deltax;
    float  cpy = -hj/2.f + deltay/2.f + py*deltay;
    float  cpz =  lookAt_.z;
    Vector3 dr = Vector3(cpx-eye_.x, cpy-eye_.y, cpz-eye_.z);
    return Ray(eye_,dr);
     */
    return Ray(eye_,(-w_)+u_*(sx*hw)+v_*(sy*hh));
}
