#ifndef LINE_HPP
#define LINE_HPP

#include "point.h"
#include "DrawSurface.hpp"

#define BLACK 0x000000
#define WHITE 0xFFFFFF
#define YELLOW 0xFFEB3B
#define GREEN 0x4CAF50
#define BLUE 0x03A9F4
#define BROWN 0x795548
#define DARKRED 0xba000d

class Line {
    public:
        Line(Point P1, Point P2);
        Line(Point P1, Point P2, uint32_t rgb);
        Line(short x1, short y1, short x2, short y2, uint32_t rgb);
        // Getters
        Point getP1();
        Point getP2();
        uint32_t getColor();
        // Draw this line
        void draw(DrawSurface& fb);
        // Move this line
        void move(short dx, short dy);
        // Rotate this line
        void rotate(float degree);
        // Scale line based on it's center point
        void scaleLine(float scaleFactor);
        // Scale line based on anchor point (ax,ay)
        void scaleLine(float scaleFactor, short ax, short ay);

        //bool intersect(short x, short y);
        bool intersect(Line line);

    private:
        Point P1;
        Point P2;
        uint32_t color;

};

#include "line.cpp"

#endif
