#include "framebuffer.h"
#include "line.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

short sign(short x)
{
    if (x > 0)
        return 1;
    else if (x < 0)
        return -1;
    else
        return 0;
}

void drawLine(FBUFFER fb, short x0, short y0, short x1, short y1, uint32_t rgb)
{
    // Check delta of x and y
    short dx = (x1 - x0);
    short dy = (y1 - y0);

    // Store absolute delta
    short absdx = abs(dx);
    short absdy = abs(dy);

    // Store double delta for optimization
    short doubledx = 2 * absdx;
    short doubledy = 2 * absdy;

    if (absdy > absdx)
    {
        short control = doubledx - absdy;

        short startY = dy > 0 ? y0 : y1;
        short endY = dy > 0 ? y1 : y0;
        short currX = dy > 0 ? x0 : x1;
        short endX = dy > 0 ? x1 : x0;

        short increment = sign(endX - currX);

        for (short y = startY; y <= endY; y++)
        {
            colorPixel(&fb, currX, y, rgb);
            control += doubledx;
            if (control > 0)
            {
                currX += increment;
                control -= doubledy;
            }
        }
    }
    else
    {
        short control = doubledy - absdx;

        short startX = dx > 0 ? x0 : x1;
        short endX = dx > 0 ? x1 : x0;
        short currY = dx > 0 ? y0 : y1;
        short endY = dx > 0 ? y1 : y0;

        short increment = sign(endY - currY);

        for (short x = startX; x <= endX; x++)
        {
            colorPixel(&fb, x, currY, rgb);
            control += doubledy;
            if (control > 0)
            {
                currY += increment;
                control -= doubledx;
            }
        }
    }
}
