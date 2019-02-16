#include "DrawSurface.hpp"
#include "line.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define PI 3.14

short sign(short x)
{
    if (x > 0)
        return 1;
    else if (x < 0)
        return -1;
    else
        return 0;
}

Line::Line(Point P1, Point P2)
{
    this->P1.x = P1.x;
    this->P2.x = P2.x;
    this->P1.y = P1.y;
    this->P2.y = P2.y;
}

Line::Line(short x1, short y1, short x2, short y2, uint32_t rgb)
{
    this->P1.x = x1;
    this->P1.y = y1;
    this->P2.x = x2;
    this->P2.y = y2;
    this->color = rgb;
}

Point Line::getP1() {
    return this->P1;
}

Point Line::getP2() {
    return this->P2;
}

void Line::draw(DrawSurface& fb)
{
    // Check delta of x and y
    short dx = (this->P2.x - this->P1.x);
    short dy = (this->P2.y - this->P1.y);

    // Store absolute delta
    short absdx = abs(dx);
    short absdy = abs(dy);

    // Store double delta for optimization
    short doubledx = 2 * absdx;
    short doubledy = 2 * absdy;

    if (absdy > absdx)
    {
        short control = doubledx - absdy;

        short startY = dy > 0 ? this->P1.y : this->P2.y;
        short endY = dy > 0 ? this->P2.y : this->P1.y;
        short currX = dy > 0 ? this->P1.x : this->P2.x;
        short endX = dy > 0 ? this->P2.x : this->P1.x;

        short increment = sign(endX - currX);

        for (short y = startY; y <= endY; y++)
        {
            fb.setPixel(currX, y, this->color);
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

        short startX = dx > 0 ? this->P1.x : this->P2.x;
        short endX = dx > 0 ? this->P2.x : this->P1.x;
        short currY = dx > 0 ? this->P1.y : this->P2.y;
        short endY = dx > 0 ? this->P2.y : this->P1.y;

        short increment = sign(endY - currY);

        for (short x = startX; x <= endX; x++)
        {
            fb.setPixel(x, currY, this->color);
            control += doubledy;
            if (control > 0)
            {
                currY += increment;
                control -= doubledx;
            }
        }
    }
}

void Line::move(short dx, short dy)
{
    this->P1.x += dx;
    this->P2.x += dx;
    this->P1.y += dy;
    this->P2.y += dy;
}

// https://www.tutorialspoint.com/computer_graphics/2d_transformation.htm
void Line::rotate(float degree)
{
    float rad = (degree/180) * PI;
    float costheta = cos(rad);
    float sintheta = sin(rad);

    short xcenter = this->P1.x;
    short ycenter = this->P1.y;
    short x2 = this->P2.x;
    short y2 = this->P2.y;

    short deltax2 = x2 - xcenter;
    short deltay2 = y2 - ycenter;
    this->P2.x = xcenter + ((deltax2 * costheta) - (deltay2 * sintheta));
    this->P2.y = ycenter + ((deltax2 * sintheta) + (deltay2 * costheta));
}

void Line::scaleLine(float scaleFactor, short ax, short ay)
{
    // Scale based on that anchor point
    short dx, dy;
    // Point #1
    dx = ax - this->P1.x;
    dy = ay - this->P1.y;
    dx *= scaleFactor;
    dy *= scaleFactor;
    this->P1.x = ax - dx;
    this->P1.y = ay - dy;
    // Point #2
    dx = ax - this->P2.x;
    dy = ay - this->P2.y;
    dx *= scaleFactor;
    dy *= scaleFactor;
    this->P2.x = ax - dx;
    this->P2.y = ay - dy;
}

void Line::scaleLine(float scaleFactor)
{
    // Find the center-point of the line
    short xc = ((this->P1.x) + (this->P2.x)) / 2;
    short yc = ((this->P1.y) + (this->P2.y)) / 2;

    this->scaleLine(scaleFactor, xc, yc);
}
