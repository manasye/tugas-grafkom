#include "line.h"
#include "framebuffer.h"
#include <math.h>
#include <stdio.h>

#define FILENAME "test.txt"

void drawOctagon(FBUFFER fb, int x0, int y0, int length, uint32_t rgb)
{
    FILE *f;
    f = fopen(FILENAME, "a");

    float rad = 0.25 * PI;

    int x01, y01, y02, x02, x_, y_;
    x_ = x0 + length;
    y_ = y0;

    drawLine(fb, x0, y0, x_, y_, rgb);
    fprintf(f, "%d,%d,%d,%d,%x\n", x0, y0, x_, y_, rgb);

    for (int i = 0; i < 3; i++)
    {
        int delta_x = round(length * cos(rad));
        int delta_y = round(length * sin(rad));

        x01 = x0 - delta_x;
        y01 = y0 + delta_y;
        x02 = x_ + delta_x;
        y02 = y_ + delta_y;

        if (x_ > x02)
        {
            printf("sisi kanan: (%d, %d) -> (%d, %d)\n", x02, y02, x_, y_);
            drawLine(fb, x02, y02, x_, y_, rgb);
            fprintf(f, "%d,%d,%d,%d,%x\n", x02, y02, x_, y_, rgb);
        } else
        {
            if (y_ < y02)
            {
                printf("sisi kanan: (%d, %d) -> (%d, %d)\n", x_, y_, x02, y02);
                drawLine(fb, x_, y_, x02, y02, rgb);
                fprintf(f, "%d,%d,%d,%d,%x\n", x_, y_, x02, y02, rgb);
            } else {
                printf("sisi kanan: (%d, %d) -> (%d, %d)\n", x02, y02, x_, y_);
                drawLine(fb, x02, y02, x_, y_, rgb);
                fprintf(f, "%d,%d,%d,%d,%x\n", x02, y02, x_, y_, rgb);
            }
        }

        if (x0 > x01)
        {
            printf("sisi kiri: (%d, %d) -> (%d, %d)\n", x01, y01, x0, y0);
            drawLine(fb, x01, y01, x0, y0, rgb);
            fprintf(f, "%d,%d,%d,%d,%x\n", x01, y01, x0, y0, rgb);
        } else
        {
            if (y0 < y01)
            {
                printf("sisi kiri: (%d, %d) -> (%d, %d)\n", x0, y0, x01, y01);
                drawLine(fb, x0, y0, x01, y01, rgb);
                fprintf(f, "%d,%d,%d,%d,%x\n", x0, y0, x01, y01, rgb);
            } else {
                printf("sisi kiri: (%d, %d) -> (%d, %d)\n", x01, y01, x0, y0);
                drawLine(fb, x01, y01, x0, y0, rgb);
                fprintf(f, "%d,%d,%d,%d,%x\n", x01, y01, x0, y0, rgb);
            }
        }

        x0 = x01;
        y0 = y01;
        x_ = x02;
        y_ = y02;

        rad += (0.25 * PI);
    }

    drawLine(fb, x0, y0, x_, y_, rgb);
    fprintf(f, "%d,%d,%d,%d,%x\n", x0, y0, x_, y_, rgb);
    fclose(f);
}

void drawTriangle(FBUFFER fb, int x0, int y0, int length, uint32_t rgb)
{
    float rad = 0.33 * PI;

    int delta_x = round(length * cos(rad));
    int delta_y = round(length * sin(rad));

    int x01, y1, x02;
    x01 = x0 - delta_x;
    y1 = y0 + delta_y;
    x02 = x0 + delta_x;

    FILE *f;
    f = fopen(FILENAME, "a");

    drawLine(fb, x01, y1, x0, y0, rgb);
    drawLine(fb, x0, y0 + (0.35 * length), x02, y1 + (0.35 * length), rgb);
    drawLine(fb, x01, y1, x02, y1, rgb);

    fprintf(f, "%d,%d,%d,%d,%x\n", x01, y1, x0, y0, rgb);
    fprintf(f, "%d,%d,%d,%d,%x\n", x0,  y0 + (int)(0.35 * length), x02, (int) y1 + (int)(0.35 * length), rgb);
    fprintf(f, "%d,%d,%d,%d,%x\n", x01, y1, x02, y1, rgb);

    fclose(f);
}
