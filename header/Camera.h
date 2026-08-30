#pragma once
#include "Ray.h"

class Camera{
    Vector3 eye_,lookAt_,up_,u_,v_,w_;
    float   fov_;
public:
    Camera();
    Camera(const Vector3&,const Vector3&,const Vector3&,float);
    void updateBasis();
    Vector3 getEye()const;
    Ray generateRay(int,int,int,int)const;
};
