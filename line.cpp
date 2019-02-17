#include "DrawSurface.hpp"
#include "line.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define PI 3.14

const short INSIDE = 0; // 0000
const short LEFT = 1;   // 0001
const short RIGHT = 2;  // 0010
const short BOTTOM = 4; // 0100
const short TOP = 8;    // 1000

//
// Helper function
//

// Sign for draw line
short sign(short x)
{
    if (x > 0)
        return 1;
    else if (x < 0)
        return -1;
    else
        return 0;
}

// Check region of certain point
// https://www.geeksforgeeks.org/line-clipping-set-1-cohen-sutherland-algorithm/
short computeCode(short x, short y, short xmin, short xmax,
                  short ymin, short ymax)
{
    short code = INSIDE;
    // Check x
    if (x < xmin)
    {
        code |= LEFT;
    }
    else if (x > xmax)
    {
        code |= RIGHT;
    }
    // Check y
    if (y < ymin)
    {
        code |= BOTTOM;
    }
    else if (y > ymax)
    {
        code |= TOP;
    }
    return code;
}

// Clip based on cohen sutherland algorithm
// https://www.geeksforgeeks.org/line-clipping-set-1-cohen-sutherland-algorithm/
Line clipCohenSutherland(DrawSurface &fb, Line &line)
{
    short x1 = line.getP1().x;
    short y1 = line.getP1().y;
    short x2 = line.getP2().x;
    short y2 = line.getP2().y;

    short xmin = 0;
    short xmax = fb.getXRes();
    short ymin = 0;
    short ymax = fb.getYRes();

    // Compute region codes for P1, P2
    short code1 = computeCode(x1, y1, xmin, xmax, ymin, ymax);
    short code2 = computeCode(x2, y2, xmin, xmax, ymin, ymax);
    bool accept = false;

    while (true)
    {
        if ((code1 == 0) && (code2 == 0))
        {
            // Both inside viewport
            accept = true;
            break;
        }
        else if (code1 & code2) 
        {
            break;
        }
        else
        {
            // Some part outside viewport, clip it!
            short codeOut;
            short x, y;

            // Pick which one is outside
            if (code1 != 0)
            {
                codeOut = code1;
            }
            else
            {
                codeOut = code2;
            }

            // If above viewport
            if (codeOut & TOP)
            {
                short ydiff = y2 - y1;
                if (ydiff == 0) 
                {
                    x = x1;
                } 
                else 
                {
                    x = x1 + (x2 - x1) * (ymax - y1) / (y2 - y1);
                }
                y = ymax;
            }
            // If below viewport
            else if (codeOut & BOTTOM)
            {
                short ydiff = y2 - y1;
                if (ydiff == 0) 
                {
                    x = x1;
                } 
                else 
                {
                    x = x1 + (x2 - x1) * (ymin - y1) / (y2 - y1);
                }
                y = ymin;
            }
            // If to right viewport
            else if (codeOut & RIGHT)
            {
                short xdiff = x2 - x1;
                if (xdiff == 0) 
                {
                    y = y1;
                } 
                else 
                {
                    y = y1 + (y2 - y1) * (xmax - x1) / (x2 - x1);
                }
                x = xmax;
            }
            // If to left viewport
            else if (codeOut & LEFT)
            {
                short xdiff = x2 - x1;
                if (xdiff == 0) {
                    y = y1;
                } 
                else 
                {
                    y = y1 + (y2 - y1) * (xmin - x1) / (x2 - x1);
                }
                x = xmin;
            }

            // Replace the "outside" part with the intersected one
            if (codeOut == code1)
            {
                x1 = x;
                y1 = y;
                code1 = computeCode(x1, y1, xmin, xmax, ymin, ymax);
            }
            else
            {
                x2 = x;
                y2 = y;
                code2 = computeCode(x2, y2, xmin, xmax, ymin, ymax);
            }
        }
    }

    if (accept) 
    {
        return Line(x1, y1, x2, y2, line.getColor());
    } 
    else 
    {
        return Line(0, 0, 0, 0, line.getColor());
    }
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

Point Line::getP1()
{
    return this->P1;
}

Point Line::getP2()
{
    return this->P2;
}

uint32_t Line::getColor()
{
    return this->color;
}

void Line::draw(DrawSurface &fb)
{
    Line toDraw = clipCohenSutherland(fb,*this);

    // Check delta of x and y
    short dx = (toDraw.getP2().x - toDraw.getP1().x);
    short dy = (toDraw.getP2().y - toDraw.getP1().y);

    // Store absolute delta
    short absdx = abs(dx);
    short absdy = abs(dy);

    // Store double delta for optimization
    short doubledx = 2 * absdx;
    short doubledy = 2 * absdy;

    if (absdy > absdx)
    {
        short control = doubledx - absdy;

        short startY = dy > 0 ? toDraw.getP1().y : toDraw.getP2().y;
        short endY = dy > 0 ? toDraw.getP2().y : toDraw.getP1().y;
        short currX = dy > 0 ? toDraw.getP1().x : toDraw.getP2().x;
        short endX = dy > 0 ? toDraw.getP2().x : toDraw.getP1().x;

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

        short startX = dx > 0 ? toDraw.getP1().x : toDraw.getP2().x;
        short endX = dx > 0 ? toDraw.getP2().x : toDraw.getP1().x;
        short currY = dx > 0 ? toDraw.getP1().y : toDraw.getP2().y;
        short endY = dx > 0 ? toDraw.getP2().y : toDraw.getP1().y;

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
    float rad = (degree / 180) * PI;
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
