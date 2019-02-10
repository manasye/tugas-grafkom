#ifndef LINE_H
#define LINE_H

#include "point.h"

#define BLACK 0x000000
#define WHITE 0xFFFFFF
#define YELLOW 0xFFEB3B
#define GREEN 0x4CAF50
#define BLUE 0x03A9F4
#define BROWN 0x795548
#define DARKRED 0xba000d

typedef struct
{
    Point P1;
    Point P2;
    uint32_t color;
} Line;

// Fills in variables in Line
Line makeLine(short x1, short y1, short x2, short y2, uint32_t rgb);

// Draws a Line
void drawLine(FBUFFER fb, short x0, short y0, short x1, short y1, uint32_t rgb);

// Draws from a Line "object"
void drawLineObject(FBUFFER * fb, Line line);

/* Line transformations */
// Translation
void moveLine(Line * line, short dx, short dy);

// Rotation
void rotateLine(Line * line, float degree);

// Scaling
void scaleLine(Line * line, float scaleFactor);

#include "line.c"

#endif
