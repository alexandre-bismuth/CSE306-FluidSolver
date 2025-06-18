#include <iostream>
#include "headers/utils.hpp"
#include "headers/config.hpp"
#include "headers/optimal_transport.hpp"

int main(int argc, char **argv)
{
    steady_clock::time_point startExecution = steady_clock::now();

    // Global variable definition
    const size_t N = 2000;
    const std::string mode = "uniform"; // Available modes : "uniform", "gaussian", "exponential"

    // Environment setup
    VoronoiDiagram Vor;
    initializePointsAndWeights(Vor, N, mode);

    // Main computations
    Vor.compute();
    OptimalTransport ot;
    ot.vor = Vor;
    ot.optimize();

    // I/O and Time computations
    save_svg(ot.vor.cells, "output/" + mode + std::to_string(N) + ".svg", &ot.vor.points);
    steady_clock::time_point endExecution = steady_clock::now();
    elapsed(startExecution, endExecution);
    return 0;
}