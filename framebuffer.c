#include "framebuffer.h"
#include <assert.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define BLACK 0x000000
#define WHITE 0xFFFFFF

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
    (*fb).buf = mmap(NULL, finfo.line_length * (*fb).yres, PROT_READ | PROT_WRITE, MAP_SHARED, temp, 0);
    assert((*fb).buf != MAP_FAILED);
}

void colorPixel(FBUFFER *fb, int x, int y, uint32_t rgb)
{
    (*fb).buf[y * (*fb).xres + x] = rgb;
}

void clear(FBUFFER *fb)
{
    int i, j;
    for (j = 0; j < (*fb).yres; j++)
    {
        for (i = 0; i < (*fb).xres; i++)
        {
            colorPixel(fb, i, j, BLACK);
        }
    }
}

void destroy(FBUFFER *fb)
{
    munmap(&((*fb).buf), (*fb).xres * 4 * (*fb).yres);
}
