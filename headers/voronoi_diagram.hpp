#ifndef VORONOI_DIAGRAM_HPP
#define VORONOI_DIAGRAM_HPP

#include "vector.hpp"
#include "polygon.hpp"

class VoronoiDiagram
{
public:
    VoronoiDiagram();

    inline void addPoint(Vector point) { points.push_back(point); };
    inline void addWeight(double weight) { weights.push_back(weight); };
    void compute();

    std::vector<Vector> points, unitDisk;
    std::vector<Polygon> cells;
    std::vector<double> weights, constWeights;
    size_t numDisks;
};

Polygon clipBisector(const Polygon &V, const Vector &P0, const Vector &Pi, double w0, double wi);
Polygon clipEdge(const Polygon &V, const Vector &P0, const Vector &Pi);

#endif