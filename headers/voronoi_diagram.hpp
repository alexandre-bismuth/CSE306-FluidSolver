#ifndef VORONOI_DIAGRAM_HPP
#define VORONOI_DIAGRAM_HPP

#include "vector.hpp"
#include "polygon.hpp"

class VoronoiDiagram
{
public:
    VoronoiDiagram() {};

    void addPoint(Vector point) { points.push_back(point); };
    void addWeight(double weight) { weights.push_back(weight); };

    Polygon clipBisector(const Polygon &V, const Vector &P0, const Vector &Pi, double w0, double wi);
    void compute();

    std::vector<Vector> points;
    std::vector<Polygon> cells;
    std::vector<double> weights;
    std::vector<double> constWeights;
};

#endif