#ifndef POLYGON_HPP
#define POLYGON_HPP

#include "point.h"
#include "DrawSurface.hpp"

class Polygon
{
  public:
    Polygon();
    void addPoint(short x, short y, uint32_t rgb);
    // Draw this polygon to framebuffer
    void draw(DrawSurface &fb);
    // Move the polygon
    void move(short dx, short dy);
    // Rotate this polygon
    void rotate(float degree);
    // Scale the polygon from it's centroid
    void scale(float scaleFactor);
    // Scale the polygon based on an anchor point (ax, ay)
    void scaleAtAnchor(float scaleFactor, short ax, short ay);
    // Rotate the polygon based on an anchor point (ax, ay)
    void rotateAtAnchor(float degree, short ax, short ay);

  private:
    Point *listOfPoint;
    uint32_t *listOfColor;
    int numOfPoint;
};

class Circle
{
  public:
    Circle(short xc, short yc, short radius, uint32_t rgb);
    // Draw this circle to framebuffer
    void draw(DrawSurface &fb);
    // Move this circle
    void move(short dx, short dy);
    // Scale this circle
    void scale(float scaleFactor);

  private:
    Point centerPoint;
    short radius;
    uint32_t color;

    void drawOtherCirclePixels(DrawSurface &fb, short x, short y);
};

#include "polygon.cpp"

#endif
