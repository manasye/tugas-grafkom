#include "line.hpp"
#include "framebuffer.hpp"
#include "polygon.hpp"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

Polygon::Polygon()
{
    this->listOfPoint = (Point *) malloc(sizeof(Point));
    this->listOfColor = (uint32_t *) malloc(sizeof(uint32_t));
    this->numOfPoint = 0;
}

void Polygon::addPoint(short x, short y, uint32_t rgb)
{
    this->listOfPoint[this->numOfPoint].x = x;
    this->listOfPoint[this->numOfPoint].y = y;
    this->listOfColor[this->numOfPoint] = rgb;
    this->listOfPoint = (Point *) realloc(this->listOfPoint, (this->numOfPoint + 2) * sizeof(Point));
    this->listOfColor = (uint32_t *) realloc(this->listOfColor, (this->numOfPoint + 2) * sizeof(uint32_t));
    this->numOfPoint++;
}

void Polygon::draw(Framebuffer& fb)
{
    if (this->numOfPoint == 1)
    {
        fb.setPixel(this->listOfPoint[0].x, this->listOfPoint[0].y, this->listOfColor[0]);
    }
    else if (numOfPoint > 1)
    {
        int i;
        Line * tempLine;
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
    for (int i = 0; i < n; i+=2) {
        Point P1 = this->listOfPoint[i];
        Point P2 = this->listOfPoint[i+1];

        Line line (P1.x, P1.y, P2.x, P2.y, this->listOfColor[i]);
        line.move(dx, dy);

        this->listOfPoint[i] = line.getP1();
        this->listOfPoint[i+1] = line.getP2();
    }
}

// Rotation
void Polygon::rotate(float degree)
{
    int n = this->numOfPoint;
    short tempx = 0;
    short tempy = 0;

    int isOdd = n % 2 == 1;
    int adjust = isOdd ? 1 : 2;
    Point P1;
    Point P2;

    for (int i = 0; i < n-adjust; i++){
        P1 = this->listOfPoint[i];
        P2 = this->listOfPoint[i+1];

        if (i > 0){
            P2.x += tempx;
            P2.y += tempy;
        }

        Line line (P1.x, P1.y, P2.x, P2.y, this->listOfColor[i]);
        line.rotate(degree);

        if (isOdd){
            tempx += line.getP2().x - P2.x;
            tempy += line.getP2().y - P2.y;
        } else {
            tempx = line.getP2().x - P2.x;
            tempy = line.getP2().y - P2.y;
        }
        this->listOfPoint[i+1] = line.getP2();
    }

    if (!isOdd){
        P1 = this->listOfPoint[0];
        P2 = this->listOfPoint[n-1];

        Line line (P1.x, P1.y, P2.x, P2.y, this->listOfColor[n-1]);
        line.rotate(degree);

        this->listOfPoint[n-1] = line.getP2();
    }
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

void Circle::drawOtherCirclePixels(Framebuffer& fb, short x, short y)
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

void Circle::draw(Framebuffer& fb)
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
