#include "edgebucket.hpp"

EdgeBucket::EdgeBucket()
{
    this->listOfEdge = (Edge *) malloc(sizeof(Edge));
    this->count = 0;
}

void EdgeBucket::addEdge(Edge e)
{
    this->listOfEdge[this->count] = e;
    this->count++;
    this->listOfEdge = (Edge *) realloc(this->listOfEdge, (this->count + 1) * sizeof(Edge));
}

Edge EdgeBucket::getEdge(int n)
{
    return this->listOfEdge[n];
}

void EdgeBucket::clear()
{
    this->count = 0;
    this->listOfEdge = (Edge *) realloc(this->listOfEdge, sizeof(Edge));
}

int EdgeBucket::size()
{
    return this->count;
}
