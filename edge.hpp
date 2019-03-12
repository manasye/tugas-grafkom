#ifndef EDGE_HPP
#define EDGE_HPP

#include "point.h"

typedef struct {
    Point P1;
    Point P2;
    int counter = 0;
    uint32_t color;
} Edge;

#endif
