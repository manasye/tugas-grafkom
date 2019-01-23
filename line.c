#include "framebuffer.h"
#include "line.h"

#define BLACK 0x000000
#define WHITE 0xFFFFFF

void drawLine(FBUFFER fb, int x0, int y0, int x1, int y1)
{
    int dx, dy, control, currX, currY, doubledx, doubledy;

    dx = x1 - x0;
    dy = y1 - y0;

    doubledx = 2 * dx;
    doubledy = 2 * dy;

    currX = x0;
    currY = y0;

    control = doubledy - dx;

    while (currX < x1)
    {
        colorPixel(&fb, currX, currY, WHITE);
        if (control > 0)
        {
            currY += 1;
            control -= doubledx;
        }
        control += doubledy;
        currX += 1;
    }
}