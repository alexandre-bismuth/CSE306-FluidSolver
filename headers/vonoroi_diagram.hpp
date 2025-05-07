#ifndef VONOROI_DIAGRAM_HPP
#define VONOROI_DIAGRAM_HPP

#include "vector.hpp"
#include "polygon.hpp"

class VonoroiDiagram
{
public:
    VonoroiDiagram() {};

    // Simple definitions are kept in the header
    void addPoint(Vector point) { points.push_back(point); };
    void addCell(Polygon cell) { cells.push_back(cell); };

    Polygon clip_by_bisector(const Polygon &V, const Vector &P0, const Vector &Pi);
    void compute();

    std::vector<Vector> points;
    std::vector<Polygon> cells;
};

#endif