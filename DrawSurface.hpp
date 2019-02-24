#ifndef __DRAWSURFACE_HPP
#define __DRAWSURFACE_HPP

#include <stdint.h>

class DrawSurface
{
public:
  short getXRes()
  {
    return xres;
  }

  short getYRes()
  {
    return yres;
  }

  uint32_t virtual getPixel(short x, short y) = 0;
  void virtual setPixel(short x, short y, uint32_t rgb) = 0;
  void virtual updateScreen() = 0;

protected:
  short yres;
  short xres;
};

#endif