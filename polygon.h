#ifndef POLYGON_H
#define POLYGON_H

#define PI 3.14159265

void drawOctagon(FBUFFER fb, int x0, int y0, int length, uint32_t rgb);
void drawTriangle(FBUFFER fb, int x0, int y0, int length, uint32_t rgb);
void drawSquare(FBUFFER fb, int x0, int y0, int side, uint32_t rgb);
void drawRectangle(FBUFFER fb, int x0, int y0, int width, int height, uint32_t rgb);
void drawHome(FBUFFER fb, int x0, int y0);
void drawTree(FBUFFER fb, int x0, int yo, int width, int height);

#include "polygon.c"
#endif
