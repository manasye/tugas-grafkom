#ifndef LINE_H
#define LINE_H

#define BLACK 0x000000
#define WHITE 0xFFFFFF
#define YELLOW 0xFFEB3B
#define GREEN 0x4CAF50
#define BLUE 0x03A9F4
#define BROWN 0x795548
#define DARKRED 0xba000d

void drawLine(FBUFFER fb, short x0, short y0, short x1, short y1, uint32_t rgb);

#include "line.c"

#endif
