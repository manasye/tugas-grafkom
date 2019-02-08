#ifndef FBUFFER_H
#define FBUFFER_H

#include <stdint.h>

// Struct definition for FBUFFER
typedef struct
{
    uint32_t * frameBuffer;
    uint32_t * backBuffer1;
    uint32_t * backBuffer2;
    uint32_t * currentBuffer;
    int xres;
    int yres;
} FBUFFER;

// YOU SHOULD ALWAYS USE THESE FUNCTIONS BELOW TO DO OPERATIONS WITH THE FRAMEBUFFER

// Initialize the framebuffer : gets info, a shared-mapping to the framebuffer, and a back-buffer
// YOU SHOULD ALWAYS CALL THIS FIRST BEFORE UTILIZING THE FRAMEBUFFER STRUCT
void init(FBUFFER *fb);

// Write a pixel in the framebuffer
void colorPixel(FBUFFER *fb, int x, int y, uint32_t rgb);

// Flush the framebuffer with black
void clear(FBUFFER *fb);

// Flush back buffer to frame-buffer
void swapBuffer(FBUFFER * fb);

// Removes the shared-mapping and deallocates back-buffer
void destroy(FBUFFER *fb);

#include "framebuffer.c"

#endif