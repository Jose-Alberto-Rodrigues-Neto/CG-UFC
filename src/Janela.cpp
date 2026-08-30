#include "Janela.h"
#include <cmath>

Janela::Janela():janela_(1.,1.,-1.){}

Janela::Janela(const Vector3&j):janela_(j){}

Vector3 Janela::getJanela()const{
    return janela_;
}
