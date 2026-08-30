#pragma once
#include "Vector3.h"

class Janela{
    Vector3 janela_;
public:
    Janela();
    Janela(const Vector3&);
    Vector3 getJanela()const;
};
