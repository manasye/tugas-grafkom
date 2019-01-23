#ifndef FBUFFER_H
#define FBUFFER_H

#include <stdint.h>

typedef struct
{
    uint32_t *buf;
    int xres;
    int yres;
} FBUFFER;

void init(FBUFFER *fb);

void colorPixel(FBUFFER *fb, int x, int y, uint32_t color);

void clear(FBUFFER *fb);

void destroy(FBUFFER *fb);

#include "framebuffer.c"

#endif