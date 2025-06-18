#include "headers/polygon.hpp"
#include "headers/utils.hpp"

double Polygon::area() const
{ // Code based on Section 4.1.1 (p. 98) of the textbook and the live coding session's code
    if (vertices.size() < 3)
        return 0.;

    double result = 0.;
    for (size_t i = 0; i < vertices.size(); ++i)
    {
        const size_t j = (i + 1) % vertices.size();
        result += vertices[i][0] * vertices[j][1] - vertices[j][0] * vertices[i][1];
    }
    return std::abs(result / 2.);
}

double Polygon::integralNorm2(const Vector &Pi) const
{ // Code based on Section 4.4.4 (p. 103) of the textbook and the live coding session's code
    if (vertices.size() < 3)
        return 0.;
    double result = 0.;
    for (size_t i = 1; i < vertices.size() - 1; ++i)
    {
        Vector triangle[3] = {vertices[0], vertices[i], vertices[i + 1]};
        double integral = 0;
        for (size_t j = 0; j < 3; ++j)
            for (size_t k = j; k < 3; ++k)
                integral += dot(triangle[j] - Pi, triangle[k] - Pi);

        Vector edge1 = triangle[1] - triangle[0], edge2 = triangle[2] - triangle[0];
        double triangleArea = std::abs(edge1[0] * edge2[1] - edge1[1] * edge2[0]) / 2.;
        result += integral * triangleArea / 6.;
    }
    return result;
}

Vector Polygon::centroid() const
{
    // Code based on Section 4.4.1 (p. 98) from the textbook
    size_t numVertices = vertices.size();
    if (numVertices == 0)
        return Vector();
    if (numVertices == 1)
        return vertices[0];
    if (numVertices == 2)
        return (vertices[0] + vertices[1]) * 0.5;

    // I follow the formulas and variables names on p.98 to the letter but
    // note that for area, we could also simply use the area() method
    double area = 0., cx = 0., cy = 0.;
    for (size_t i = 0; i < numVertices; ++i)
    {
        const Vector &x = vertices[i], &y = vertices[(i + 1) % numVertices];
        double crossProduct = cross(x, y)[2];
        area += crossProduct / 2.;
        cx += (x[0] + y[0]) * crossProduct;
        cy += (x[1] + y[1]) * crossProduct;
    }
    return Vector(cx / (6. * area), cy / (6. * area));
}
