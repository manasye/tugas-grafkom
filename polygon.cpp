#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <iostream>
#include "line.hpp"
#include "DrawSurface.hpp"
#include "polygon.hpp"

/*                
    Helper function  
*/

// Pseudocode taken from https://graphics.fandom.com/wiki/Flood_fill
// Target color is the initial color (in our case is BLACK)
// Replacement color is the new color
void floodFill(DrawSurface &fb, short x, short y,
               uint32_t targetColor, uint32_t replacementColor)
{

    // Initialize queue
    typedef std::pair<short, short> pairedShort;
    std::list<std::pair<short, short>> toBeColored;

    // Not equal to target color
    if (fb.getPixel(x, y) != targetColor)
        return;

    // Push if it is to be colored
    toBeColored.push_back(pairedShort(x, y));

    while (!toBeColored.empty())
    {
        // Get current element
        pairedShort currCoor = toBeColored.front();
        toBeColored.pop_front();

        // Get current coordinate
        short x = currCoor.first;
        short y = currCoor.second;

        // Set the color
        fb.setPixel(x, y, replacementColor);

        // West area
        if (fb.getPixel(x - 1, y) == targetColor)
        {
            toBeColored.push_back(pairedShort(x - 1, y));
        }
        // East area
        if (fb.getPixel(x + 1, y) == targetColor)
        {
            toBeColored.push_back(pairedShort(x + 1, y));
        }
        // North area
        if (fb.getPixel(x, y - 1) == targetColor)
        {
            toBeColored.push_back(pairedShort(x, y - 1));
        }
        // South area
        if (fb.getPixel(x, y + 1) == targetColor)
        {
            toBeColored.push_back(pairedShort(x, y + 1));
        }
        toBeColored.unique();
        //printf("%d\n",toBeColored.size());
    }
}

void floodFillRecursive(DrawSurface &fb, short x, short y,
                        uint32_t targetColor, uint32_t replacementColor)
{
    // Not equal to target color
    if (fb.getPixel(x, y) != targetColor)
        return;

    // Set the color
    fb.setPixel(x, y, replacementColor);

    // West area
    if (fb.getPixel(x - 1, y) == targetColor)
    {
        floodFillRecursive(fb, x - 1, y, targetColor, replacementColor);
    }
    // East area
    if (fb.getPixel(x + 1, y) == targetColor)
    {
        floodFillRecursive(fb, x + 1, y, targetColor, replacementColor);
    }
    // North area
    if (fb.getPixel(x, y - 1) == targetColor)
    {
        floodFillRecursive(fb, x, y - 1, targetColor, replacementColor);
    }
    // South area
    if (fb.getPixel(x, y + 1) == targetColor)
    {
        floodFillRecursive(fb, x, y + 1, targetColor, replacementColor);
    }
}

bool isCorner(DrawSurface &fb, short x, short y, uint32_t color)
{
    short i = x;
    while (i < fb.getXRes())
    {
        if (fb.getPixel(i + 1, y) == color)
        {
            return false;
        }
        i++;
    }
    return true;
}

void scanLine(DrawSurface &fb, uint32_t targetColor)
{
    for (short i = 0; i < fb.getYRes(); i++)
    {
        short j = 0;
        while (j < fb.getXRes())
        {
            if ((fb.getPixel(j, i) != targetColor))
            {
                if (!isCorner(fb, j, i, fb.getPixel(j, i)))
                {
                    uint32_t temp = fb.getPixel(j, i);
                    int count = 0;
                    while ((fb.getPixel(j + 1, i) != temp) && (j < fb.getXRes()))
                    {
                        count++;
                        fb.setPixel(j + 1, i, temp);
                        j++;
                    }
                }
            }
            j++;
        }
    }
}

Polygon::Polygon()
{
    this->listOfPoint = (Point *)malloc(sizeof(Point));
    this->listOfColor = (uint32_t *)malloc(sizeof(uint32_t));
    this->numOfPoint = 0;
}

uint32_t Polygon::getFirstColor()
{
    return this->listOfColor[0];
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

        Point center = this->calculateCentroid();

        if (!this->contains(center))
        {
            Point left = this->leftmostPoint();
            left.y = center.y;
            left.x += 2;
            /*
            while (!this->contains(left)) {
                left.x++;
            }
            */
            center = left;
        }

        // printf("Coloring polygon at (%d,%d)\n", center.x, center.y);
        floodFillRecursive(fb, center.x, center.y, BLACK, listOfColor[0]);
        // scanLine(fb, BLACK);
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
    Point center = this->calculateCentroid();

    // Scale based on centroid
    this->rotateAtAnchor(degree, center.x, center.y);
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
    Point centroid = this->calculateCentroid();
    // Scale based on centroid
    this->scaleAtAnchor(scaleFactor, centroid.x, centroid.y);
}

Point Polygon::leftmostPoint()
{
    Point leftmost = listOfPoint[0];
    for (int i = 1; i < this->numOfPoint; i++)
    {
        if (leftmost.x > listOfPoint[i].x)
        {
            leftmost = listOfPoint[i];
        }
    }
    return leftmost;
}

Point Polygon::calculateCentroid()
{
    Point center;
    center.x = 0;
    center.y = 0;

    for (int i = 0; i < this->numOfPoint; i++)
    {
        center.x += this->listOfPoint[i].x;
        center.y += this->listOfPoint[i].y;
    }
    center.x = center.x / this->numOfPoint;
    center.y = center.y / this->numOfPoint;

    return center;
}

bool Polygon::contains(short x, short y)
{
    Point temp;
    temp.x = x;
    temp.y = y;
    return this->contains(temp);
}

/* Using the virtual ray algorithm
   Idea : Create a line from any point outside the polygon and the point in question.
          Count the number of intersection with the polygon's sides.
          If it's odd, the point is inside the polygon.
*/
bool Polygon::contains(Point p)
{
    Point startPoint = this->leftmostPoint();
    startPoint.x -= 5;
    startPoint.y = p.y;

    Line vRay(startPoint, p);
    Line *tempLine;
    int intersectCount = 0;

    for (int i = 0; i < numOfPoint - 1; i++)
    {
        tempLine = new Line(this->listOfPoint[i], this->listOfPoint[i + 1], this->listOfColor[i]);
        if (tempLine->intersect(vRay))
        {
            intersectCount++;
        }
        delete tempLine;
    }
    tempLine = new Line(this->listOfPoint[this->numOfPoint - 1], this->listOfPoint[0], this->listOfColor[numOfPoint - 1]);
    if (tempLine->intersect(vRay))
    {
        intersectCount++;
    }
    delete tempLine;

    if ((intersectCount % 2) == 0)
    {
        return false;
    }
    else
    {
        return true;
    }
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
