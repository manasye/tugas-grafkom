#ifndef POLYGON_H
#define POLYGON_H

#define PI 3.14159265

typedef struct {
    int x;
    int y;
} Point;

void drawOctagon(FBUFFER fb, int x0, int y0, int length, uint32_t rgb);
void drawTriangle(FBUFFER fb, int x0, int y0, int length, uint32_t rgb);
void drawSquare(FBUFFER fb, int x0, int y0, int side, uint32_t rgb);
void drawRectangle(FBUFFER fb, int x0, int y0, int width, int height, uint32_t rgb);
void drawHome(FBUFFER fb, int x0, int y0);
void drawTree(FBUFFER fb, int x0, int yo, int width, int height);

// Sets the framebuffer to draw and prepares the program to start storing control points
void drawStart(FBUFFER *fb);

// Add a control point
// The color passed will be used for drawing the next line
void drawAddPoint(int x, int y, uint32_t rgb);

// Finishes the preparation and flushes the changes to framebuffer
void drawEnd();

#include "polygon.c"
#endif
