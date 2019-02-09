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

void drawAddPoint(short x, short y, uint32_t rgb)
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

void addPointPolygon(Polygon * poly, short x, short y, uint32_t rgb)
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

void movePolygon(Polygon * poly, short dx, short dy)
{
    // replace this with the implementation
}

// Rotation
void rotatePolygon(Polygon * poly, short degree)
{
     // replace this with the implementation
}

// Scaling
void scalePolygon(Polygon * poly, short scaleFactor)
{
    // replace this with the implementation
}

Circle makeCircle(short xc, short yc, short radius, uint32_t rgb)
{  
    Circle circle;
    circle.centerPoint.x = xc;
    circle.centerPoint.y = yc;
    circle.radius = radius;
    circle.color = rgb;
    return circle;
}

void drawOtherCirclePixels(FBUFFER fb, short xc, short yc, short x, short y, uint32_t rgb)
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

void drawCircle(FBUFFER fb, short xc, short yc, short r, uint32_t rgb)
{
    short x = 0;
    short y = r;
    short d = 3 - 2 * r;
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

void drawCircleObject(FBUFFER * fb, Circle circle) {
    drawCircle((*fb), circle.centerPoint.x, circle.centerPoint.y, circle.radius, circle.color);
}

void moveCircle(Circle * circle, short dx, short dy)
{
    // replace this with the implementation
}

void scaleCircle(Circle * circle, short scaleFactor)
{
    // replace this with the implementation
}