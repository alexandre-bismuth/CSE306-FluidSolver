#ifndef FLUID_HPP
#define FLUID_HPP

#include "voronoi_diagram.hpp"
#include "utils.hpp"
#include "optimal_transport.hpp"
#include "vector.hpp"

class Fluid
{
public:
    Fluid(size_t N, size_t numFluidCells, double mass, double eps, double dt);

    void stepGallouetMerigot();
    void simulate();

    OptimalTransport ot;
    std::vector<Vector> particles;
    std::vector<Vector> velocities;
    std::vector<Vector> colors;

    double mass, eps, dt;
    size_t N, numFluidCells;
};

#endif