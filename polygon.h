#ifndef POLYGON_H
#define POLYGON_H

#define PI 3.14159265

typedef struct
{
    int x;
    int y;
} Point;

// Sets the framebuffer to draw and prepares the program to start storing control points
void drawStart(FBUFFER *fb);

// Add a control point (x,y)
// The color rgb passed will be used for drawing the next line
void drawAddPoint(int x, int y, uint32_t rgb);

// Finishes the preparation and flushes the changes to framebuffer
void drawEnd();

// Draw a circle, with center-point (xc,yc), radius r, and colored rgb
void drawCircle(FBUFFER fb, int xc, int yc, int r, uint32_t rgb);

#include "polygon.c"
#endif
