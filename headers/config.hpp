#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "vector.hpp"
#include "voronoi_diagram.hpp"

inline void normalizeWeights(std::vector<double> &weights)
{
    double sum = 0.;
    for (double weight : weights)
        sum += weight;
    if (sum == 0.)
        throw std::runtime_error("Zero sum of weights prevents normalization");
    for (double &weight : weights)
        weight /= sum;
}

inline void initializePointsAndWeights(VoronoiDiagram &Vor, size_t N, const std::string &mode)
{
    Vor.points.reserve(N);
    Vor.weights.reserve(N);
    Vor.constWeights.reserve(N);
    for (size_t i = 0; i < N; i++)
    {
        Vector point = Vector(uniform(engine), uniform(engine), 0.);
        Vor.addPoint(point);
        if (mode == "uniform")
            Vor.addWeight(1);
        else if (mode == "gaussian")
        {
            // Mean is the square's center and we set the sigma = 0.18 from visual output
            // Exponent from Gaussian distribution formula
            double exponent = -(point - Vector(0.5, 0.5)).norm2() / (2 * sqr(0.18));
            Vor.addWeight(std::exp(exponent));
        }
        else if (mode == "exponential")
        {
            // Exponential distribution from bottom left corner with parameter 4
            Vor.addWeight(std::exp(-4 * point.norm()));
        }
        else
            throw std::runtime_error("Chosen mode is invalid");
    }
    normalizeWeights(Vor.weights);
    Vor.constWeights = Vor.weights;
}

#endif