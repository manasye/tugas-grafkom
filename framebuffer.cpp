#include "framebuffer.hpp"
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

void clearBuffer(uint32_t *buf, int xres, int yres)
{
    int i, j;
    for (j = 0; j < yres; j++)
    {
        for (i = 0; i < xres; i++)
        {
            buf[j * xres + i] = BLACK;
        }
    }
}

int rgbToHex(int r, int g, int b)
{
    return (r << 16) | (g << 8) | b;
}

Framebuffer::Framebuffer()
{
    // Getting the framebuffer
    int temp = open("/dev/fb0", O_RDWR);
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    assert(temp > 0);

    // Getting framebuffer information - width, height
    assert(ioctl(temp, FBIOGET_VSCREENINFO, &vinfo) == 0);
    assert(ioctl(temp, FBIOGET_FSCREENINFO, &finfo) == 0);
    this->xres = finfo.line_length / 4;
    this->yres = vinfo.yres_virtual;

    // Getting the framebuffer memory
    this->frameBuffer = (uint32_t *)mmap(NULL, finfo.line_length * this->yres, PROT_READ | PROT_WRITE, MAP_SHARED, temp, 0);
    assert(this->frameBuffer != MAP_FAILED);

    // Allocates back buffer
    this->backBuffer1 = (uint32_t *)malloc(finfo.line_length * this->yres);
    clearBuffer(this->backBuffer1, this->xres, this->yres);
    this->backBuffer2 = (uint32_t *)malloc(finfo.line_length * this->yres);
    clearBuffer(this->backBuffer2, this->xres, this->yres);

    // Sets current back buffer to the first one
    this->currentBuffer = this->backBuffer1;

    // Initialize flushThread
    this->firstTime = 1;
}

Framebuffer::~Framebuffer()
{
    if (!this->firstTime)
    {
        this->flushThread->join();
    }
    munmap(&(this->frameBuffer), this->xres * 4 * this->yres);
    free(this->backBuffer1);
    free(this->backBuffer2);
}

void Framebuffer::setPixel(short x, short y, uint32_t rgb)
{
    if ((x >= 0) && (x < this->xres) && (y >= 0) && (y < this->yres))
    {
        this->currentBuffer[y * this->xres + x] = rgb;
    }
}

void Framebuffer::clear()
{
    clearBuffer(this->currentBuffer, this->xres, this->yres);
}

void Framebuffer::flush()
{
    if (this->currentBuffer == this->backBuffer1)
    {
        memcpy(this->frameBuffer, this->backBuffer2, this->xres * 4 * this->yres);
        clearBuffer(this->backBuffer2, this->xres, this->yres);
    }
    else
    {
        memcpy(this->frameBuffer, this->backBuffer1, this->xres * 4 * this->yres);
        clearBuffer(this->backBuffer1, this->xres, this->yres);
    }
}

void Framebuffer::updateScreen()
{
    if (!this->firstTime)
    {
        this->flushThread->join();
    }
    else
    {
        this->firstTime = 0;
    }

    if (this->currentBuffer == this->backBuffer1)
    {
        this->currentBuffer = this->backBuffer2;
    }
    else
    {
        this->currentBuffer = this->backBuffer1;
    }

    this->flushThread = new std::thread(&Framebuffer::flush, this);
}
