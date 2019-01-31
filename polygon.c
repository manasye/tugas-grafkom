#include "line.h"
#include "framebuffer.h"
#include "polygon.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define FILENAME "test.txt"

FBUFFER *currBuffer;
Point *pointList;
int numOfPoint;
uint32_t *colorList;

void drawOctagon(FBUFFER fb, int x0, int y0, int length, uint32_t rgb)
{
    FILE *f;
    f = fopen(FILENAME, "a");

    float rad = 0.25 * PI;

    int x01, y01, y02, x02, x_, y_;
    x_ = x0 + length;
    y_ = y0;

    int x_rad_top_bot = x0 + round(length / 2);
    int lengthRad = 25;

    drawLine(fb, x0, y0, x_, y_, rgb);
    drawLine(fb, x_rad_top_bot, y0 - 35, x_rad_top_bot, y0 - 10, rgb);
    fprintf(f, "%d,%d,%d,%d,%x\n", x0, y0, x_, y_, rgb);
    fprintf(f, "%d,%d,%d,%d,%x\n", x_rad_top_bot, y0 - 35, x_rad_top_bot, y0 - 10, rgb);

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
        }
        else
        {
            if (y_ < y02)
            {
                printf("sisi kanan: (%d, %d) -> (%d, %d)\n", x_, y_, x02, y02);
                drawLine(fb, x_, y_, x02, y02, rgb);
                fprintf(f, "%d,%d,%d,%d,%x\n", x_, y_, x02, y02, rgb);
            }
            else
            {
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
        }
        else
        {
            if (y0 < y01)
            {
                printf("sisi kiri: (%d, %d) -> (%d, %d)\n", x0, y0, x01, y01);
                drawLine(fb, x0, y0, x01, y01, rgb);
                fprintf(f, "%d,%d,%d,%d,%x\n", x0, y0, x01, y01, rgb);
            }
            else
            {
                printf("sisi kiri: (%d, %d) -> (%d, %d)\n", x01, y01, x0, y0);
                drawLine(fb, x01, y01, x0, y0, rgb);
                fprintf(f, "%d,%d,%d,%d,%x\n", x01, y01, x0, y0, rgb);
            }
        }

        //draw Radiation
        int x_radiation_1, x_radiation_2;
        int y_radiation = y_ + round(delta_y / 2);

        if (delta_x > 0)
        {
            x_radiation_1 = x_ + round(delta_x / 2) + 35;
            x_radiation_2 = x0 - round(delta_x / 2) - 35;
            y_radiation -= lengthRad;
            drawLine(fb, x_radiation_1 - lengthRad, y_radiation + lengthRad, x_radiation_1, y_radiation, rgb);
            drawLine(fb, x_radiation_2, y_radiation, x_radiation_2 + lengthRad, y_radiation + lengthRad, rgb);

            fprintf(f, "%d,%d,%d,%d,%x\n", x_radiation_1 - lengthRad, y_radiation + lengthRad, x_radiation_1, y_radiation, rgb);
            fprintf(f, "%d,%d,%d,%d,%x\n", x_radiation_2, y_radiation, x_radiation_2 + lengthRad, y_radiation + lengthRad, rgb);
        }
        else if (delta_x < 0)
        {
            x_radiation_1 = x_ + round(delta_x / 2) + 35;
            x_radiation_2 = x0 - round(delta_x / 2) - 35;
            drawLine(fb, x_radiation_1 - lengthRad, y_radiation, x_radiation_1, y_radiation + lengthRad, rgb);
            drawLine(fb, x_radiation_2, y_radiation + lengthRad, x_radiation_2 + lengthRad, y_radiation, rgb);

            fprintf(f, "%d,%d,%d,%d,%x\n", x_radiation_1 - lengthRad, y_radiation, x_radiation_1, y_radiation + lengthRad, rgb);
            fprintf(f, "%d,%d,%d,%d,%x\n", x_radiation_2, y_radiation + lengthRad, x_radiation_2 + lengthRad, y_radiation, rgb);
        }
        else
        {
            x_radiation_1 = x02 + 35;
            x_radiation_2 = x01 - 35;
            drawLine(fb, x_radiation_1 - lengthRad, y_radiation, x_radiation_1, y_radiation, rgb);
            drawLine(fb, x_radiation_2, y_radiation, x_radiation_2 + lengthRad, y_radiation, rgb);

            fprintf(f, "%d,%d,%d,%d,%x\n", x_radiation_1 - lengthRad, y_radiation, x_radiation_1, y_radiation, rgb);
            fprintf(f, "%d,%d,%d,%d,%x\n", x_radiation_2, y_radiation, x_radiation_2 + lengthRad, y_radiation, rgb);
        }

        x0 = x01;
        y0 = y01;
        x_ = x02;
        y_ = y02;

        rad += (0.25 * PI);
    }

    drawLine(fb, x0, y0, x_, y_, rgb);
    fprintf(f, "%d,%d,%d,%d,%x\n", x0, y0, x_, y_, rgb);

    drawLine(fb, x_rad_top_bot, y0 + 10, x_rad_top_bot, y0 + 35, rgb);
    fprintf(f, "%d,%d,%d,%d,%x\n", x_rad_top_bot, y0 + 10, x_rad_top_bot, y0 + 35, rgb);

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
    fprintf(f, "%d,%d,%d,%d,%x\n", x0, y0 + (int)(0.35 * length), x02, (int)y1 + (int)(0.35 * length), rgb);
    fprintf(f, "%d,%d,%d,%d,%x\n", x01, y1, x02, y1, rgb);

    fclose(f);
}

void drawSquare(FBUFFER fb, int x0, int y0, int side, uint32_t rgb)
{
    FILE *f;
    f = fopen(FILENAME, "a");

    drawLine(fb, x0, y0, x0 + side, y0, rgb);
    drawLine(fb, x0, y0, x0, y0 + side, rgb);
    drawLine(fb, x0 + side, y0, x0 + side, y0 + side, rgb);
    drawLine(fb, x0, y0 + side, x0 + side, y0 + side, rgb);
    fprintf(f, "%d,%d,%d,%d,%x\n", x0, y0, x0 + side, y0, rgb);
    fprintf(f, "%d,%d,%d,%d,%x\n", x0, y0, x0, y0 + side, rgb);
    fprintf(f, "%d,%d,%d,%d,%x\n", x0 + side, y0, x0 + side, y0 + side, rgb);
    fprintf(f, "%d,%d,%d,%d,%x\n", x0, y0 + side, x0 + side, y0 + side, rgb);

    fclose(f);
}

void drawRectangle(FBUFFER fb, int x0, int y0, int width, int height, uint32_t rgb)
{
    FILE *f;
    f = fopen(FILENAME, "a");

    int topRight = x0 + width;
    int bottomRight = y0 + height;

    drawLine(fb, x0, y0, topRight, y0, rgb);
    drawLine(fb, x0, y0, x0, bottomRight, rgb);
    drawLine(fb, topRight, y0, topRight, bottomRight, rgb);
    drawLine(fb, x0, bottomRight, topRight, bottomRight, rgb);

    fprintf(f, "%d,%d,%d,%d,%x\n", x0, y0, topRight, y0, rgb);
    fprintf(f, "%d,%d,%d,%d,%x\n", x0, y0, x0, bottomRight, rgb);
    fprintf(f, "%d,%d,%d,%d,%x\n", topRight, y0, topRight, bottomRight, rgb);
    fprintf(f, "%d,%d,%d,%d,%x\n", x0, bottomRight, topRight, bottomRight, rgb);

    fclose(f);
}

void drawHome(FBUFFER fb, int x0, int y0)
{
    //draw Roof
    drawTriangle(fb, x0 + 50, y0 - 87, 100, DARKRED);

    //draw Home
    drawSquare(fb, x0, y0, 100, BROWN);

    //draw Window
    int topLeft = x0 + 65;
    int bottomRight = y0 + 20;
    int x_vertical = x0 + 65 + 13;
    int y_vertical = y0 + 20;
    int x_horizontal = x0 + 65;
    int y_horizontal = y0 + 20 + 13;

    drawSquare(fb, topLeft, bottomRight, 25, BROWN);
    drawLine(fb, x_vertical, y_vertical, x_vertical, y_vertical + 25, BROWN);
    drawLine(fb, x_horizontal, y_horizontal, x_horizontal + 25, y_horizontal, BROWN);

    FILE *f;
    f = fopen(FILENAME, "a");
    fprintf(f, "%d,%d,%d,%d,%x\n", x_vertical, y_vertical, x_vertical, y_vertical + 25, BROWN);
    fprintf(f, "%d,%d,%d,%d,%x\n", x_horizontal, y_horizontal, x_horizontal + 25, y_horizontal, BROWN);
    fclose(f);

    //draw Door
    drawRectangle(fb, x0 + 25, y0 + 100 - 40, 30, 40, BROWN);
}

void drawTree(FBUFFER fb, int x0, int y0, int width, int height)
{
    int x_ = x0 + round(width / 2);
    int y_ = y0 - round(0.86 * height);

    // draw trunk
    drawRectangle(fb, x0, y0, width, height, BROWN);

    // draw leaf
    drawTriangle(fb, x_, y_, height, GREEN);
}

void drawStart(FBUFFER *fb)
{
    currBuffer = fb;
    numOfPoint = 0;
    pointList = (Point *)malloc(sizeof(Point));
    colorList = (uint32_t *)malloc(sizeof(uint32_t));
}

void drawAddPoint(int x, int y, uint32_t rgb)
{
    numOfPoint++;
    pointList = (Point *)realloc(pointList, (numOfPoint + 1) * sizeof(Point));
    colorList = (uint32_t *)realloc(colorList, (numOfPoint + 1) * sizeof(uint32_t));
    pointList[numOfPoint - 1].x = x;
    pointList[numOfPoint - 1].y = y;
    colorList[numOfPoint - 1] = rgb;
}

void drawEnd()
{
    if (numOfPoint == 1)
    {
        colorPixel(currBuffer, pointList[0].x, pointList[0].y, colorList[0]);
    }
    else
    {
        int i;
        for (i = 0; i < numOfPoint - 1; i++)
        {
            drawLine(*currBuffer, pointList[i].x, pointList[i].y, pointList[i + 1].x, pointList[i + 1].y, colorList[i]);
        }
        drawLine(*currBuffer, pointList[numOfPoint - 1].x, pointList[numOfPoint - 1].y, pointList[0].x, pointList[0].y, colorList[numOfPoint - 1]);
    }
}

void drawOtherCirclePixels(FBUFFER fb, int xc, int yc, int x, int y, uint32_t rgb)
{
    colorPixel(&fb, xc + x, yc + y, rgb);
    colorPixel(&fb, xc - x, yc + y, rgb);
    colorPixel(&fb, xc + x, yc - y, rgb);
    colorPixel(&fb, xc - x, yc - y, rgb);
    colorPixel(&fb, xc + y, yc + x, rgb);
    colorPixel(&fb, xc - y, yc + x, rgb);
    colorPixel(&fb, xc + y, yc - x, rgb);
    colorPixel(&fb, xc - y, yc - x, rgb);
}

// Courtesy of https://www.geeksforgeeks.org/bresenhams-circle-drawing-algorithm/

void drawCircle(FBUFFER fb, int xc, int yc, int r, uint32_t rgb)
{
    int x = 0;
    int y = r;
    int d = 3 - 2 * r;
    drawOtherCirclePixels(fb, xc, yc, x, y, rgb);
    while (y >= x)
    {
        x++;
        if (d > 0)
        {
            y--;
            d = d + 4 * (x - y) + 10;
        }
        else
            d = d + 4 * x + 6;
        drawOtherCirclePixels(fb, xc, yc, x, y, rgb);
    }
}