#include "framebuffer.h"
#include "line.h"

#define BLACK 0x000000
#define WHITE 0xFFFFFF

void drawLine(FBUFFER fb, int x0, int y0, int x1, int y1, uint32_t rgb)
{
    int dx, dy, control, currX, currY, doubledx, doubledy;

    // Check delta of x and y
    dx = x1 - x0;
    dy = y1 - y0;

    // Horizontal line
    if (dy == 0)
    {
        for (int i = x0; i <= x1; i++)
        {
            colorPixel(&fb, i, y0, rgb);
        }
    }
    // Vertical line
    else if (dx == 0)
    {
        for (int i = y0; i <= y1; i++)
        {
            colorPixel(&fb, x0, i, rgb);
        }
    }
    // Line that have gradient
    else
    {
        // Store double delta for optimizaiton
        doubledx = 2 * dx;
        doubledy = 2 * dy;

        currX = x0;
        currY = y0;
        control = doubledy - dx;

        while (currX < x1)
        {
            colorPixel(&fb, currX, currY, rgb);

            if (control > 0)
            {
                currY += 1;
                control -= doubledx;
            }

            control += doubledy;
            currX += 1;
        }
    }
}