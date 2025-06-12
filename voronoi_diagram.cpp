#include "headers/voronoi_diagram.hpp"

Polygon VoronoiDiagram::clipBisector(const Polygon &V, const Vector &P0, const Vector &Pi)
{
    Polygon result;
    result.vertices.reserve(V.vertices.size() + 1);
    for (size_t i = 0; i < V.vertices.size(); i++)
    {
        const Vector &A = V.vertices[(i == 0) ? V.vertices.size() - 1 : i - 1];
        const Vector &B = V.vertices[i];

        if ((B - P0).norm2() <= (B - Pi).norm2()) // B inside
        {
            if ((A - P0).norm2() > (A - Pi).norm2()) // A outside
            {
                Vector M = (P0 + Pi) * 0.5;
                double t = dot(M - A, Pi - P0) / dot(B - A, Pi - P0);
                Vector P = A + t * (B - A);
                result.addVertex(P);
            }
            result.addVertex(B);
        }
        else
        {
            if ((A - P0).norm2() <= (A - Pi).norm2()) // A inside
            {
                Vector M = (P0 + Pi) * 0.5;
                double t = dot(M - A, Pi - P0) / dot(B - A, Pi - P0);
                Vector P = A + t * (B - A);
                result.addVertex(P);
            }
        }
    }
    return result;
}

void VoronoiDiagram::compute()
{
    Polygon square;
    square.addVertex(Vector(0, 0));
    square.addVertex(Vector(0, 1));
    square.addVertex(Vector(1, 1));
    square.addVertex(Vector(1, 0));

    cells.resize(points.size());

#pragma omp parallel for schedule(dynamic, 1)
    for (size_t i = 0; i < points.size(); i++)
    {
        Polygon V = square;
        for (size_t j = 0; j < points.size(); j++)
        {
            if (i == j)
                continue;
            V = clipBisector(V, points[i], points[j]);
        }
        cells[i] = V;
    }
}