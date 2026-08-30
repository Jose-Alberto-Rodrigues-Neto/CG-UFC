#pragma once

#include "Framebuffer.h"

class Display
{
public:
    static void show(
        const Framebuffer& framebuffer,
        const char* title);
};
