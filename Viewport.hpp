#ifndef __VIEWPORT_HPP
#define __VIEWPORT_HPP

#include "DrawSurface.hpp"
#include "framebuffer.hpp"

#include <stdint.h>

// Viewport : a small window to be drawn to a framebuffer
class Viewport : public DrawSurface
{
  public:
    Viewport(Framebuffer *fb, int xres, int yres);
    Viewport(Framebuffer *fb, int xres, int yres, short xloc, short yloc);
    ~Viewport();

    // Getter & setters
    Framebuffer *getFramebuffer();
    void bindToFramebuffer(Framebuffer *fb);
    short getViewportXPos();
    void setViewportXPos(short xloc);
    short getViewportYPos();
    void setViewportYPos(short yloc);

    // Clears the buffer
    void clear();
    // Flushes to the framebuffer (without updating the screen)
    void flushToFramebuffer();

    // Implementing methods from DrawSurface
    void setPixel(short x, short y, uint32_t rgb);
    // Flush to framebuffer, and updates the screen
    void updateScreen();

  private:
    Framebuffer *fb;
    uint32_t *buffer;
    // Position of viewport on framebuffer (top left)
    short xloc;
    short yloc;
};

#include "Viewport.cpp"

#endif