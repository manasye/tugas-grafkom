#ifndef POLYGON_H
#define POLYGON_H

#define PI 3.14159265

void drawOctagon(FBUFFER fb, int x0, int y0, int length, uint32_t rgb);
void drawTriangle(FBUFFER fb, int x0, int y0, int length, uint32_t rgb);

#include "polygon.c"
#endif
