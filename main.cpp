#include <iostream>
#include "headers/utils.hpp"
#include "headers/vonoroi_diagram.hpp"
#include "svg_writer.cpp"

using namespace std::chrono;

int main(int argc, char **argv) // Optionally add execution arguments later
{
    const steady_clock::time_point start = steady_clock::now();

    // Global variable definition
    int N = 10000;

    // Environment setup
    VonoroiDiagram Vor;
    Vor.points.reserve(N);

    // Main computations
#pragma omp parallel for schedule(dynamic, 1)
    for (int i = 0; i < N; i++)
    {
        Vor.addPoint(Vector(uniform(engine), uniform(engine), 0.0));
    }
    Vor.compute();

    save_svg(Vor.cells, "output/test_output.svg", &Vor.points);

    // Time computations
    const steady_clock::time_point end = steady_clock::now();
    double elapsed = duration<double>(end - start).count() / 1000.;
    std::cout << "A-Z Execution time : " << elapsed << " seconds" << std::endl;

    return 0;
}
