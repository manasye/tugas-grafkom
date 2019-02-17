#include "framebuffer.hpp"
#include "Viewport.hpp"

#include <stdio.h>
#include <stdlib.h>

Viewport::Viewport(Framebuffer *fb, int xres, int yres)
{
    this->fb = fb;
    this->xres = xres;
    this->yres = yres;
    this->buffer = (uint32_t *)malloc(sizeof(uint32_t) * xres * yres);
    this->xloc = 0;
    this->yloc = 0;
}

Viewport::Viewport(Framebuffer *fb, int xres, int yres, short xloc, short yloc)
{
    this->fb = fb;
    this->xres = xres;
    this->yres = yres;
    this->buffer = (uint32_t *)malloc(sizeof(uint32_t) * xres * yres);
    this->xloc = xloc;
    this->yloc = yloc;
}

Viewport::~Viewport()
{
    free(this->buffer);
}

Framebuffer *Viewport::getFramebuffer()
{
    return this->fb;
}

void Viewport::bindToFramebuffer(Framebuffer *fb)
{
    this->fb = fb;
}

short Viewport::getViewportXPos()
{
    return this->xloc;
}

void Viewport::setViewportXPos(short xloc)
{
    this->xloc = xloc;
}

short Viewport::getViewportYPos()
{
    return this->yloc;
}

void Viewport::setViewportYPos(short yloc)
{
    this->yloc = yloc;
}

void Viewport::clear()
{
    for (int j = 0; j < this->yres; j++)
    {
        for (int i = 0; i < this->xres; i++)
        {
            this->buffer[j * this->xres + i] = 0;
        }
    }
}

void Viewport::setPixel(short x, short y, uint32_t rgb)
{
    if ((x >= 0) && (x < this->xres) && (y >= 0) && (y < this->yres))
    {
        this->buffer[y * this->xres + x] = rgb;
    }
}

void Viewport::flushToFramebuffer()
{
    // Also draws a white border to signify the viewport
    for (int i = -1; i <= this->xres; i++)
    {
        this->fb->setPixel(this->xloc + i, this->yloc - 1, 0xFFFFFF);
    }

    int j;
    for (j = 0; j < this->yres; j++)
    {
        this->fb->setPixel(this->xloc - 1, this->yloc + j, 0xFFFFFF);
        for (int i = 0; i < this->xres; i++)
        {
            this->fb->setPixel(this->xloc + i, this->yloc + j, this->buffer[j * this->xres + i]);
        }
        this->fb->setPixel(this->xloc + this->xres, this->yloc + j, 0xFFFFFF);
    }

    for (int i = -1; i <= this->xres; i++)
    {
        this->fb->setPixel(this->xloc + i, this->yloc + j, 0xFFFFFF);
    }

    this->clear();
}

void Viewport::updateScreen()
{
    this->flushToFramebuffer();
    this->fb->updateScreen();
}