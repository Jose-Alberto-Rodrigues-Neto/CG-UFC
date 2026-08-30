#include "SceneLoader.h"
#include "Sphere.h"
#include <fstream>
#include <stdexcept>

static Vector3 v3(std::istream&i){
    float x,y,z;
    i>>x>>y>>z;
    return{x,y,z};
}

static Color col(std::istream&i){
    float r,g,b;
    i>>r>>g>>b;
    return{r,g,b};
}

Scene SceneLoader::loadFromFile(const std::string&f){
    std::ifstream in(f);
    if(!in) throw std::runtime_error("Nao abriu: "+f);
    Scene s;
    std::string t;
    while(in>>t){
        if(t[0] == '#'){
            std::string line;
            std::getline(in,line);
            continue;
        }
        
        if(t=="camera"){
            auto  e=v3(in),l=v3(in),u=v3(in);
            float fov;
            in>>fov;
            s.camera = Camera(e,l,u,fov);
        }
        else if(t=="janela"){
            auto  j=v3(in);
            s.janela = Janela(j);
        }
        else if (t=="ambient")    s.ambientLight = col(in);
        else if (t=="background") s.background   = col(in);
        else if (t=="light"){
            auto p=v3(in);
            auto c=col(in);
            s.lights.emplace_back(p,c);
        }else if (t=="sphere"){
            auto c=v3(in);
            float r;
            in>>r;
            auto ka=col(in),kd=col(in),ks=col(in);
            float n;
            in>>n;
            s.objects.push_back(std::make_shared<Sphere>(c,r,Material(ka,kd,ks,n)));
        }else throw std::runtime_error("Token desconhecido: "+t);}return s;}
