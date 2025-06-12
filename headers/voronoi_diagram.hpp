#ifndef VORONOI_DIAGRAM_HPP
#define VORONOI_DIAGRAM_HPP

#include "vector.hpp"
#include "polygon.hpp"

class VoronoiDiagram
{
public:
    VoronoiDiagram() {};

    void addPoint(Vector point) { points.push_back(point); };
    void addCell(Polygon cell) { cells.push_back(cell); };

    Polygon clipBisector(const Polygon &V, const Vector &P0, const Vector &Pi);
    void compute();

    std::vector<Vector> points;
    std::vector<Polygon> cells;
};

#endif