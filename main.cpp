#include <iostream>
#include "headers/utils.hpp"
#include "headers/config.hpp"
#include "headers/optimal_transport.hpp"
#include "headers/fluid.hpp"

int main(int argc, char **argv)
{
    steady_clock::time_point startExecution = steady_clock::now();

    // Global variable definition
    const size_t N = 500, numFluidCells = 500;
    const double mass = 200, eps = 0.004, dt = 0.002;

    // Environment setup
    Fluid fluid(N, numFluidCells, mass, eps, dt);

    // Main computation
    fluid.simulate();

    // I/O and Time computations
    steady_clock::time_point endExecution = steady_clock::now();
    elapsed(startExecution, endExecution);
    return 0;
}