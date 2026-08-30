#pragma once
#include "Scene.h"
#include "Framebuffer.h"

class Renderer{
    float eps_=1e-4f;
    bool  closestHit(const Scene&,const Ray&,HitRecord&)const;
    bool  isInShadow(const Scene&,const Vector3&,const Vector3&,const PointLight&)const;
    Color shade(const Scene&,const Ray&,const HitRecord&)const;
public:
    void render(const Scene&,Framebuffer&)const;
};
