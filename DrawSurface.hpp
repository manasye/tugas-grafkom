#ifndef __DRAWSURFACE_HPP
#define __DRAWSURFACE_HPP

#include <stdint.h>

class DrawSurface
{
  public:
    int getXRes()
    {
        return xres;
    }

    int getYRes()
    {
        return yres;
    }

    void virtual setPixel(short x, short y, uint32_t rgb) = 0;
    void virtual updateScreen() = 0;

  protected:
    int yres;
    int xres;
};

#endif