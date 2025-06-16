#ifndef POLYGON_HPP
#define POLYGON_HPP

#include "vector.hpp"

class Polygon
{
public:
    void addVertex(Vector vertex) { vertices.push_back(vertex); }
    double area() const;
    double integralNorm2(const Vector &Pi) const;
    std::vector<Vector> vertices;
};

#endif