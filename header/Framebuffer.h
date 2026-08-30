#pragma once

#include <vector>
#include <cstdint>

#include "Color.h"


class Framebuffer
{
private:

    int width_;
    int height_;

    std::vector<std::uint8_t> pixels_;

public:

    Framebuffer(int width, int height);

    int width() const
    {
        return width_;
    }

    int height() const
    {
        return height_;
    }


    void setPixel(int x, int y, const Color& color
    );

    const std::uint8_t* data() const
    {
        return pixels_.data();
    }


    std::uint8_t* data()
    {
        return pixels_.data();
    }
};
