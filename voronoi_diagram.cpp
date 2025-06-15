#include "headers/voronoi_diagram.hpp"

bool inside(const Vector &V, const Vector &P0, const Vector &Pi) { return (V - P0).norm2() <= (V - Pi).norm2(); }
bool outside(const Vector &V, const Vector &P0, const Vector &Pi) { return (V - P0).norm2() > (V - Pi).norm2(); }

Polygon VoronoiDiagram::clipBisector(const Polygon &V, const Vector &P0, const Vector &Pi)
{
    Polygon result;
    result.vertices.reserve(V.vertices.size() + 1);
    for (size_t i = 0; i < V.vertices.size(); i++)
    {
        const Vector &A = V.vertices[(i == 0) ? V.vertices.size() - 1 : i - 1];
        const Vector &B = V.vertices[i];

        if (inside(B, P0, Pi))
        {
            if (outside(A, P0, Pi))
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
            if (inside(A, P0, Pi))
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
    Polygon unitSquare;
    unitSquare.addVertex(Vector(0, 0));
    unitSquare.addVertex(Vector(0, 1));
    unitSquare.addVertex(Vector(1, 1));
    unitSquare.addVertex(Vector(1, 0));

    cells.resize(points.size());

#pragma omp parallel for schedule(dynamic, 1)
    for (size_t i = 0; i < points.size(); i++)
    { // Algorithm from Section 4.3.3 of the textbook
        // Here, I also implement the optional early-exit from Figure 4.9
        Polygon V = unitSquare;
        double R = std::sqrt(2); // Since we use a unit square
        for (size_t j = 0; j < points.size(); j++)
        {
            if (i == j)
                continue;
            if ((points[i] - points[j]).norm() > 2. * R)
                continue;

            V = clipBisector(V, points[i], points[j]);

            R = 0.;
            for (const Vector &vertex : V.vertices)
                R = std::max(R, (vertex - points[i]).norm());
        }
        cells[i] = std::move(V); // std::move here allows for O(1) instead of O(N) malloc
    }
}