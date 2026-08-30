#include "Renderer.h"
#include <limits>
#include <cmath>
#include <iostream>


bool Renderer::closestHit(const Scene&s,const Ray&r,HitRecord&hit)const{
    // STUDENT TODO 3: escolher a intersecao mais proxima.
    bool  found   = false;
    float closest = std::numeric_limits<float>::infinity();
    
    for(const auto&o:s.objects){
        HitRecord h;
        if(o->intersect(r,eps_,closest,h)){
            found   = true;
            closest = h.t;
            hit     = h;
        }
    }
    return found;
}

bool Renderer::isInShadow(const Scene&s,const Vector3&p,const Vector3&n,const PointLight&l)const{
 // STUDENT TODO 4: raio de sombra.
    Vector3 d    = l.position()-p;
    float   dist = d.length();
    Ray     sr(p+n*eps_,d/dist);
    
    for (const auto&o:s.objects){
        HitRecord h;
        if(o->intersect(sr,eps_,dist-eps_,h)) return true;
    }
    return false;
}

Color Renderer::shade(const Scene&s, const Ray& r, const HitRecord& h)const{
 // STUDENT TODO 5: modelo de Phong (ambiente + difusa + especular).
    const Material&m = *h.material;
    Color out        = m.ka*s.ambientLight;
    Vector3 N        = h.normal.normalized();
    Vector3 V        = (-r.direction).normalized();
    
    for (const auto&l:s.lights){
        if (isInShadow(s,h.point,N,l)) continue;
        Vector3 L     = (l.position() - h.point).normalized();
        float   ndotl = std::max(0.f,N.dot(L));
        Color   diff  = (m.kd*l.intensity())*ndotl;
        Vector3 R     = (2.f*N.dot(L)*N - L).normalized();
        float   rv    = std::max(0.f,R.dot(V));
        Color   spec  = (m.ks*l.intensity())*std::pow(rv,m.shininess);
        out          += diff + spec;
    }
    return out.clamped();
}
void Renderer::render(const Scene&s,Framebuffer&fb)const{
 // STUDENT TODO 6: loop sobre pixels.
    float wj = s.janela.getJanela().x; //2.f;
    float hj = s.janela.getJanela().y; //2.f;
    float delx = wj/fb.width();
    float dely = hj/fb.height();
    
    Vector3 cm;
    Vector3 dr;
    
    cm.z = s.janela.getJanela().z; //-3.f;
    std::cout << " H " << fb.height() << std::endl;
    std::cout << " W " << fb.width() << std::endl;
    
    for (int y=0; y<fb.height();++y)
    {
        cm.y = hj/2 - dely/2.f - dely*y;
        
        for(int x=0;x<fb.width();++x){
            cm.x = -wj/2.f + delx/2.f + delx*x;
            dr = cm - s.camera.getEye();
            dr = dr.normalized();
            //Ray       r = s.camera.generateRay(x,y,fb.width(),fb.height());
            Ray r = Ray(s.camera.getEye(), dr);
            
            HitRecord h;
            fb.setPixel(x,y,closestHit(s,r,h)?shade(s,r,h):s.background);
        }
    }
}
