#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <iostream>
#include "line.hpp"
#include "DrawSurface.hpp"
#include "polygon.hpp"

using namespace std;

/*                
    Helper function  
*/

// Pseudocode taken from https://graphics.fandom.com/wiki/Flood_fill
// Target color is the initial color (in our case is BLACK)
// Replacement color is the new color
void floodFill(DrawSurface &fb, double x, double y,
               uint32_t targetColor, uint32_t replacementColor)
{
    // Initialize queue
    typedef pair<double, double> pairedDouble;
    queue<pair<double, double>> toBeColored;

    // Not equal to target color
    if (fb.getPixel(x, y) != targetColor)
        return;

    // Push if it is to be colored
    toBeColored.push(pairedDouble(x, y));

    // Loop until all to be colored empty
    while (!toBeColored.empty())
    {
        // Get current element
        pairedDouble currCoor = toBeColored.front();
        toBeColored.pop();
        // Get current coordinate
        double x = currCoor.first;
        double y = currCoor.second;
        // Set the color
        fb.setPixel(x, y, replacementColor);
        // West area
        if (fb.getPixel(x - 1, y) == targetColor)
            toBeColored.push(pairedDouble(x - 1, y));
        // East area
        if (fb.getPixel(x + 1, y) == targetColor)
            toBeColored.push(pairedDouble(x + 1, y));
        // North area
        if (fb.getPixel(x, y - 1) == targetColor)
            toBeColored.push(pairedDouble(x, y - 1));
        // South area
        if (fb.getPixel(x, y + 1) == targetColor)
            toBeColored.push(pairedDouble(x, y + 1));
    }
}

Polygon::Polygon()
{
    this->listOfPoint = (Point *)malloc(sizeof(Point));
    this->listOfColor = (uint32_t *)malloc(sizeof(uint32_t));
    this->numOfPoint = 0;
}

void Polygon::addPoint(short x, short y, uint32_t rgb)
{
    this->listOfPoint[this->numOfPoint].x = x;
    this->listOfPoint[this->numOfPoint].y = y;
    this->listOfColor[this->numOfPoint] = rgb;
    this->listOfPoint = (Point *)realloc(this->listOfPoint, (this->numOfPoint + 2) * sizeof(Point));
    this->listOfColor = (uint32_t *)realloc(this->listOfColor, (this->numOfPoint + 2) * sizeof(uint32_t));
    this->numOfPoint++;
}

void Polygon::draw(DrawSurface &fb)
{
    if (this->numOfPoint == 1)
    {
        fb.setPixel(this->listOfPoint[0].x, this->listOfPoint[0].y, this->listOfColor[0]);
    }
    else if (numOfPoint > 1)
    {
        int i;
        Line *tempLine;
        for (i = 0; i < numOfPoint - 1; i++)
        {
            tempLine = new Line(this->listOfPoint[i].x, this->listOfPoint[i].y, this->listOfPoint[i + 1].x, this->listOfPoint[i + 1].y, this->listOfColor[i]);
            tempLine->draw(fb);
            delete tempLine;
        }
        tempLine = new Line(this->listOfPoint[this->numOfPoint - 1].x, this->listOfPoint[numOfPoint - 1].y, this->listOfPoint[0].x, this->listOfPoint[0].y, this->listOfColor[numOfPoint - 1]);
        tempLine->draw(fb);
        delete tempLine;
    }
}

void Polygon::move(short dx, short dy)
{
    int n = this->numOfPoint;
    for (int i = 0; i < n; i += 2)
    {
        Point P1 = this->listOfPoint[i];
        Point P2 = this->listOfPoint[i + 1];

        Line line(P1.x, P1.y, P2.x, P2.y, this->listOfColor[i]);
        line.move(dx, dy);

        this->listOfPoint[i] = line.getP1();
        this->listOfPoint[i + 1] = line.getP2();
    }
}

// Rotation
void Polygon::rotateAtAnchor(float degree, short ax, short ay)
{
    Point P1;
    P1.x = ax;
    P1.y = ay;

    int n = this->numOfPoint;

    for (int i = 0; i < n; i++)
    {
        Point P2 = this->listOfPoint[i];

        Line line(P1, P2);
        line.rotate(degree);

        this->listOfPoint[i] = line.getP2();
    }
}

void Polygon::rotate(float degree)
{
    int xc = 0;
    int yc = 0;
    for (int i = 0; i < this->numOfPoint; i++)
    {
        xc += this->listOfPoint[i].x;
        yc += this->listOfPoint[i].y;
    }

    xc = xc / this->numOfPoint;
    yc = yc / this->numOfPoint;

    // Scale based on centroid
    this->rotateAtAnchor(degree, xc, yc);
}

// Scaling
void Polygon::scaleAtAnchor(float scaleFactor, short ax, short ay)
{
    for (int i = 0; i < this->numOfPoint; i++)
    {
        int dx = ax - this->listOfPoint[i].x;
        int dy = ay - this->listOfPoint[i].y;
        dx *= scaleFactor;
        dy *= scaleFactor;
        this->listOfPoint[i].x = ax - dx;
        this->listOfPoint[i].y = ay - dy;
    }
}

void Polygon::scale(float scaleFactor)
{
    // Find centroid
    int xc = 0;
    int yc = 0;
    for (int i = 0; i < this->numOfPoint; i++)
    {
        xc += this->listOfPoint[i].x;
        yc += this->listOfPoint[i].y;
    }
    xc = xc / this->numOfPoint;
    yc = yc / this->numOfPoint;

    // Scale based on centroid
    this->scaleAtAnchor(scaleFactor, xc, yc);
}

Circle::Circle(short xc, short yc, short radius, uint32_t rgb)
{
    this->centerPoint.x = xc;
    this->centerPoint.y = yc;
    this->radius = radius;
    this->color = rgb;
}

void Circle::drawOtherCirclePixels(DrawSurface &fb, short x, short y)
{
    fb.setPixel(this->centerPoint.x + x, this->centerPoint.y + y, this->color);
    fb.setPixel(this->centerPoint.x - x, this->centerPoint.y + y, this->color);
    fb.setPixel(this->centerPoint.x + x, this->centerPoint.y - y, this->color);
    fb.setPixel(this->centerPoint.x - x, this->centerPoint.y - y, this->color);
    fb.setPixel(this->centerPoint.x + y, this->centerPoint.y + x, this->color);
    fb.setPixel(this->centerPoint.x - y, this->centerPoint.y + x, this->color);
    fb.setPixel(this->centerPoint.x + y, this->centerPoint.y - x, this->color);
    fb.setPixel(this->centerPoint.x - y, this->centerPoint.y - x, this->color);
}

// Courtesy of https://www.geeksforgeeks.org/bresenhams-circle-drawing-algorithm/

void Circle::draw(DrawSurface &fb)
{
    short x = 0;
    short y = this->radius;
    short d = 3 - 2 * this->radius;
    this->drawOtherCirclePixels(fb, x, y);
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
        this->drawOtherCirclePixels(fb, x, y);
    }
}

void Circle::move(short dx, short dy)
{
    this->centerPoint.x += dx;
    this->centerPoint.y += dy;
}

void Circle::scale(float scaleFactor)
{
    this->radius *= scaleFactor;
}
