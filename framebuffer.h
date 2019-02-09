#ifndef FBUFFER_H
#define FBUFFER_H

#include <stdint.h>
#include <pthread.h>

// Struct definition for FBUFFER
typedef struct
{
    uint32_t * frameBuffer;
    uint32_t * backBuffer1;
    uint32_t * backBuffer2;
    uint32_t * currentBuffer;
    int xres;
    int yres;
    pthread_t flushThread; // DO NOT ACCESS IT DIRECTLY, EXCEPT FROM THESE FUNCTIONS BELOW
    int firstTime;
} FBUFFER;

// YOU SHOULD ALWAYS USE THESE FUNCTIONS BELOW TO DO OPERATIONS WITH THE FRAMEBUFFER

// Initialize the framebuffer : gets info, a shared-mapping to the framebuffer, and a back-buffer
// YOU SHOULD ALWAYS CALL THIS FIRST BEFORE UTILIZING THE FRAMEBUFFER STRUCT
void init(FBUFFER *fb);

// Write a pixel in the framebuffer
// If x or y is out-of-bounds, this function will do nothing
void colorPixel(FBUFFER *fb, short x, short y, uint32_t rgb);

// Flush the framebuffer with black
void clear(FBUFFER *fb);

// Flush current buffer to frame buffer and swaps the current buffer
void swapBuffer(FBUFFER * fb);

// Removes the shared-mapping and deallocates back-buffer
void destroy(FBUFFER *fb);

#include "framebuffer.c"

#endif