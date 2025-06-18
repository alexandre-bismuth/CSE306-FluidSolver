#include "headers/voronoi_diagram.hpp"

VoronoiDiagram::VoronoiDiagram()
{
    numDisks = 50;
    unitDisk.resize(numDisks);
    for (size_t i = 0; i < numDisks; i++)
    {
        double theta = 2 * M_PI * i / (double)numDisks;
        unitDisk[i] = Vector(cos(theta), sin(theta));
    }
}

// Helper functions for clipping
bool inside(const Vector &V, const Vector &P0, const Vector &Pi, double w0, double wi)
{
    return (V - P0).norm2() - w0 <= (V - Pi).norm2() - wi;
}
bool outside(const Vector &V, const Vector &P0, const Vector &Pi, double w0, double wi)
{
    return (V - P0).norm2() - w0 > (V - Pi).norm2() - wi; // Or, more simply (but less clearly), !inside(...)
}
Vector intersect(const Vector &A, const Vector &B, const Vector &u, const Vector &N)
{
    // Taken from the formula P = A + t(B − A) with t = ⟨u − A,N⟩ on page 88
    double t = dot(u - A, N) / dot(B - A, N);
    return A + t * (B - A);
}

Polygon clipBisector(const Polygon &V, const Vector &P0, const Vector &Pi, double w0, double wi)
{ // Code based on Section 4.3.3 (p. 93-95) of the textbook and the live coding session's code
    Polygon result;
    result.vertices.reserve(V.vertices.size() + 1);
    Vector M = (P0 + Pi) * 0.5;
    Vector MPrime = M + (w0 - wi) / (2 * (Pi - P0).norm2()) * (Pi - P0);
    for (size_t i = 0; i < V.vertices.size(); ++i)
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

Polygon clipEdge(const Polygon &subjectPolygon, const Vector &u, const Vector &v)
{
    // Code based on Section 4.2, Algorithm 4 (p. 87-88) of the textbook
    const size_t numVertices = subjectPolygon.vertices.size();
    Polygon result;
    result.vertices.reserve(2 * numVertices);
    const Vector normal = Vector(v[1] - u[1], u[0] - v[0]);
    for (size_t i = 0; i < numVertices; ++i)
    {
        const Vector curVertex = subjectPolygon.vertices[i];
        const Vector prevVertex = subjectPolygon.vertices[(i > 0) ? (i - 1) : (numVertices - 1)];
        const Vector intersection = intersect(prevVertex, curVertex, u, normal);
        if (dot(curVertex - u, normal) <= 0)
        {
            if (dot(prevVertex - u, normal) > 0)
                result.addVertex(intersection);
            result.addVertex(curVertex);
        }
        else if (dot(prevVertex - u, normal) <= 0)
            result.addVertex(intersection);
    }
    return result;
}

void VoronoiDiagram::compute()
{ // Code based on Section 4.3.3 (p. 93-95) of the textbook and the live coding session's code
  // I also implement the optional early-exit from Figure 4.9
    Polygon unitSquare;
    unitSquare.addVertex(Vector(0, 0));
    unitSquare.addVertex(Vector(1, 0));
    unitSquare.addVertex(Vector(1, 1));
    unitSquare.addVertex(Vector(0, 1));

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
        for (size_t j = 0; j < numDisks; ++j)
        {
            double radius = sqrt(weights[i] - weights[weights.size() - 1]);
            Vector u = unitDisk[j] * radius + points[i];
            Vector v = unitDisk[(j + 1) % numDisks] * radius + points[i];
            V = clipEdge(V, u, v);
        }

        cells[i] = std::move(V); // std::move here allows for O(1) instead of O(N) malloc
    }
}