#include "headers/voronoi_diagram.hpp"

// Helper functions for clipping
bool inside(const Vector &V, const Vector &P0, const Vector &Pi, double w0, double wi)
{
    return (V - P0).norm2() - w0 <= (V - Pi).norm2() - wi;
}
bool outside(const Vector &V, const Vector &P0, const Vector &Pi, double w0, double wi)
{
    return (V - P0).norm2() - w0 > (V - Pi).norm2() - wi; // Or, more simply (but less clearly), !inside(...)
}

Polygon VoronoiDiagram::clipBisector(const Polygon &V, const Vector &P0, const Vector &Pi, double w0, double wi)
{ // Code based on Section 4.3.3 (p. 93-95) of the textbook
    Polygon result;
    result.vertices.reserve(V.vertices.size() + 1);
    Vector M = (P0 + Pi) * 0.5;
    Vector MPrime = M + (w0 - wi) / (2 * (Pi - P0).norm2()) * (Pi - P0);
    for (size_t i = 0; i < V.vertices.size(); i++)
    {
        const Vector &A = V.vertices[(i == 0) ? V.vertices.size() - 1 : i - 1];
        const Vector &B = V.vertices[i];

        if (inside(B, P0, Pi, w0, wi))
        {
            if (outside(A, P0, Pi, w0, wi))
            {
                double t = dot(MPrime - A, Pi - P0) / dot(B - A, Pi - P0);
                Vector P = A + t * (B - A);
                result.addVertex(P);
            }
            result.addVertex(B);
        }
        else
        {
            if (inside(A, P0, Pi, w0, wi))
            {
                double t = dot(MPrime - A, Pi - P0) / dot(B - A, Pi - P0);
                Vector P = A + t * (B - A);
                result.addVertex(P);
            }
        }
    }
    return result;
}

void VoronoiDiagram::compute()
{ // Code based on Section 4.3.3 (p. 93-95) of the textbook
  // I also implement the optional early-exit from Figure 4.9
    Polygon unitSquare;
    unitSquare.addVertex(Vector(0, 0));
    unitSquare.addVertex(Vector(0, 1));
    unitSquare.addVertex(Vector(1, 1));
    unitSquare.addVertex(Vector(1, 0));

    cells.resize(points.size());

#pragma omp parallel for schedule(dynamic, 1)
    for (size_t i = 0; i < points.size(); ++i)
    {
        Polygon V = unitSquare;
        double R = std::sqrt(2); // Since we use a unit square
        for (size_t j = 0; j < points.size(); ++j)
        {
            if (i == j || (points[i] - points[j]).norm() > 2. * R)
                continue;

            V = clipBisector(V, points[i], points[j], weights[i], weights[j]);

            R = 0.;
            for (const Vector &vertex : V.vertices)
                R = std::max(R, (vertex - points[i]).norm());
        }
        cells[i] = std::move(V); // std::move here allows for O(1) instead of O(N) malloc
    }
}