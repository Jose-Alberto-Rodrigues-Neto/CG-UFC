#pragma once
#include <memory>
#include <vector>
#include "Camera.h"
#include "Janela.h"
#include "Object.h"
#include "PointLight.h"

class Scene{
public:
    Camera camera;
    Janela janela;
    Color  ambientLight{.1f,.1f,.1f};
    Color  background{.03f,.04f,.08f};
    std::vector<std::shared_ptr<Object>> objects;
    std::vector<PointLight> lights;
};
