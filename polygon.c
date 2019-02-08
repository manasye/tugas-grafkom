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
    else if (numOfPoint > 1)
    {
        int i;
        for (i = 0; i < numOfPoint - 1; i++)
        {
            drawLine(*currBuffer, pointList[i].x, pointList[i].y, pointList[i + 1].x, pointList[i + 1].y, colorList[i]);
        }
        drawLine(*currBuffer, pointList[numOfPoint - 1].x, pointList[numOfPoint - 1].y, pointList[0].x, pointList[0].y, colorList[numOfPoint - 1]);
    }
}

void initPolygon(Polygon * poly)
{
    (*poly).listOfPoint = (Point *) malloc(sizeof(Point));
    (*poly).listOfColors = malloc(sizeof(uint32_t));
    (*poly).numOfPoint = 0;
}

void addPointPolygon(Polygon * poly, int x, int y, uint32_t rgb)
{
    (*poly).listOfPoint[(*poly).numOfPoint].x = x;
    (*poly).listOfPoint[(*poly).numOfPoint].y = y;
    (*poly).listOfColors[(*poly).numOfPoint] = rgb;
    (*poly).listOfPoint = (Point *) realloc((*poly).listOfPoint, ((*poly).numOfPoint + 2) * sizeof(Point));
    (*poly).listOfColors = (uint32_t *) realloc((*poly).listOfColors, ((*poly).numOfPoint + 2) * sizeof(uint32_t)); 
    (*poly).numOfPoint++;
}

void drawPolygon(FBUFFER * fb, Polygon poly)
{
    drawStart(fb);
    int i;
    for (i = 0; i < poly.numOfPoint; i++) {
        drawAddPoint(poly.listOfPoint[i].x, poly.listOfPoint[i].y, poly.listOfColors[i]);
    }
    drawEnd();
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