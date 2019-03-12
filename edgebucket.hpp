#ifndef EDGEBUCKET_HPP
#define EDGEBUCKET_HPP

#include "edge.hpp"

class EdgeBucket
{
    public:
        EdgeBucket();
        void addEdge(Edge e);
        int size();
        void clear();
        Edge getEdge(int n);
    private:
        Edge* listOfEdge;
        int count;
};

#include "edgebucket.cpp"

#endif
