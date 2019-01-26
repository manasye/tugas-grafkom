#ifndef FBUFFER_H
#define FBUFFER_H

#include <stdint.h>

// Struct definition for FBUFFER
typedef struct
{
    uint32_t *buf;
    int xres;
    int yres;
} FBUFFER;

// Initialize the framebuffer : gets info & a shared-mapping to the framebuffer
// YOU SHOULD ALWAYS CALL THIS FIRST BEFORE UTILIZING THE FRAMEBUFFER STRUCT
void init(FBUFFER *fb);

// Write a pixel in the framebuffer
void colorPixel(FBUFFER *fb, int x, int y, uint32_t rgb);

// Flush the framebuffer with black
void clear(FBUFFER *fb);

// Removes the shared-mapping
void destroy(FBUFFER *fb);

#include "framebuffer.c"

#endif