#pragma once
#include <string>
#include "Scene.h"

class SceneLoader{
public:
    static Scene loadFromFile(const std::string&);
};
