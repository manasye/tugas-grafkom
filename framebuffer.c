#include "framebuffer.h"
#include <assert.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BLACK 0x000000
#define WHITE 0xFFFFFF

void clearBuffer(uint32_t * buf, int xres, int yres) {
    int i, j;
    for (j = 0; j < yres; j++)
    {
        for (i = 0; i < xres; i++)
        {
            buf[j * xres + i] = BLACK;
        }
    }
}

void init(FBUFFER *fb)
{
    // Getting the framebuffer
    int temp = open("/dev/fb0", O_RDWR);
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    assert(temp > 0);
    // Getting framebuffer information - width, height
    assert(ioctl(temp, FBIOGET_VSCREENINFO, &vinfo) == 0);
    assert(ioctl(temp, FBIOGET_FSCREENINFO, &finfo) == 0);
    (*fb).xres = finfo.line_length / 4;
    (*fb).yres = vinfo.yres_virtual;
    // Getting the framebuffer memory
    (*fb).frameBuffer = mmap(NULL, finfo.line_length * (*fb).yres, PROT_READ | PROT_WRITE, MAP_SHARED, temp, 0);
    assert((*fb).frameBuffer != MAP_FAILED);
    // Allocates back buffer
    (*fb).backBuffer1 = malloc(finfo.line_length * (*fb).yres);
    clearBuffer((*fb).backBuffer1,(*fb).xres, (*fb).yres);
    (*fb).backBuffer2 = malloc(finfo.line_length * (*fb).yres);
    clearBuffer((*fb).backBuffer2,(*fb).xres, (*fb).yres);
    // Sets current back buffer to the first one
    (*fb).currentBuffer = (*fb).backBuffer1;
    // Initialize flushThread
    (*fb).firstTime = 1;
}

int rgbToHex(int r, int g, int b)
{
    return (r << 16) | (g << 8) | b;
}

void colorPixel(FBUFFER *fb, short x, short y, uint32_t rgb)
{   
    if ((x >= 0) && (x < (*fb).xres) && (y >= 0) && (y < (*fb).yres)) 
    {
        (*fb).currentBuffer[y * (*fb).xres + x] = rgb;
    }
}

void clear(FBUFFER *fb)
{
    clearBuffer((*fb).currentBuffer, (*fb).xres, (*fb).yres);
}

void flush(FBUFFER *fb) 
{
    if ((*fb).currentBuffer == (*fb).backBuffer1) {
        memcpy((*fb).frameBuffer, (*fb).backBuffer2, (*fb).xres * 4 * (*fb).yres);
        clearBuffer((*fb).backBuffer2,(*fb).xres, (*fb).yres);
    } else {
        memcpy((*fb).frameBuffer, (*fb).backBuffer1, (*fb).xres * 4 * (*fb).yres);
        clearBuffer((*fb).backBuffer1,(*fb).xres, (*fb).yres);
    }
}

void swapBuffer(FBUFFER * fb) 
{
    if (!(*fb).firstTime) {
        pthread_join((*fb).flushThread, NULL);
    } else {
        (*fb).firstTime = 0;
    }

    if ((*fb).currentBuffer == (*fb).backBuffer1) {
        (*fb).currentBuffer = (*fb).backBuffer2;
    } else {
        (*fb).currentBuffer = (*fb).backBuffer1;
    }
    
    if (pthread_create(&(*fb).flushThread, NULL, flush, (void *) fb)) {
        printf("Error creating thread to flush framebuffer...");
        exit(EXIT_FAILURE);
    }
}

void destroy(FBUFFER *fb)
{
    if (!(*fb).firstTime) {
        pthread_join((*fb).flushThread, NULL);
    }
    munmap(&((*fb).frameBuffer), (*fb).xres * 4 * (*fb).yres);
    free((*fb).backBuffer1);
    free((*fb).backBuffer2);
}
