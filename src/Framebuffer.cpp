#include "Framebuffer.h"

Framebuffer::Framebuffer(int width, int height): width_(width), height_(height), pixels_(static_cast<std::size_t>(width*height*3),0)
{
}

void Framebuffer::setPixel(int x, int y, const Color& color)
{
    if (x < 0 || x >= width_ || y < 0 || y >= height_)
    {
        return;
    }


    Color c = color.clamped();


    std::size_t index = static_cast<std::size_t>((y * width_ + x) * 3);


    pixels_[index + 0] = static_cast<std::uint8_t>(255.0f * c.r);
    pixels_[index + 1] = static_cast<std::uint8_t>(255.0f * c.g);
    pixels_[index + 2] = static_cast<std::uint8_t>(255.0f * c.b);
}
