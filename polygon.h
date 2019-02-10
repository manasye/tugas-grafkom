#ifndef POLYGON_H
#define POLYGON_H

#include "point.h"

typedef struct
{
    Point * listOfPoint;
    uint32_t * listOfColors;
    int numOfPoint;
} Polygon;

typedef struct
{
    Point centerPoint;
    short radius;
    uint32_t color;
} Circle;

/* POLYGON FUNCTIONS */

// Sets the framebuffer to draw and prepares the program to start storing control points
void drawStart(FBUFFER *fb);

// Add a control point (x,y)
// The color rgb passed will be used for drawing the next line
void drawAddPoint(short x, short y, uint32_t rgb);

// Finishes the preparation and flushes the changes to framebuffer
void drawEnd();

// Initialize Polygon
void initPolygon(Polygon * poly);

// Add points to Polygon
void addPointPolygon(Polygon * poly, short x, short y, uint32_t rgb);

// Draws a Polygon
void drawPolygon(FBUFFER * fb, Polygon poly);

/* Polygon Transformations */
// Translation
void movePolygon(Polygon * poly, short dx, short dy);

// Rotation
void rotatePolygon(Polygon * poly, float degree);

// Scaling
void scalePolygon(Polygon * poly, float scaleFactor);

/* CIRCLE FUNCTIONS */

// Initialize a Circle
Circle makeCircle(short xc, short yc, short radius, uint32_t rgb);

// Draw a circle, with center-point (xc,yc), radius r, and colored rgb
void drawCircle(FBUFFER fb, short xc, short yc, short r, uint32_t rgb);

// Draw a circle from Circle "object"
void drawCircleObject(FBUFFER * fb, Circle circle);

/* Circle Transformations */
// Translation
void moveCircle(Circle * circle, short dx, short dy);

// Scaling
void scaleCircle(Circle * circle, float scaleFactor);

#include "polygon.c"
#endif
