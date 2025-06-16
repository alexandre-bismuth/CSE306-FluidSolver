#include "headers/polygon.hpp"
#include "headers/utils.hpp"

double Polygon::area() const
{ // Code based on Section 4.1.1 (p. 98) of the textbook
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
{ // Code based on Section 4.4.4 (p. 103) of the textbook
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