#ifndef UTILS_HPP
#define UTILS_HPP

#define M_PI 3.14159265358979323846264338327950288
#define VOL_FLUID 0.6

#include <algorithm>
#include <random>
#include <iostream>
#include "polygon.hpp"
#include "vector.hpp"

using steady_clock = std::chrono::steady_clock;

static std::default_random_engine engine;
static std::uniform_real_distribution<double> uniform;

inline double sqr(double x) { return x * x; };
inline double clamp(double x, double floor, double ceil) { return std::max(std::min(ceil, x), floor); }

inline int sgn(double x)
{
    if (x > 0)
        return 1;
    else if (x < 0)
        return -1;
    else
        return 0;
}
inline void elapsed(steady_clock::time_point &start, steady_clock::time_point &end)
{
    double elapsed = std::chrono::duration<double>(end - start).count();
    std::cout << "Execution Time: " << elapsed << " seconds" << std::endl;
}

void save_svg_animated(const std::vector<Polygon> &polygons, std::string filename, int frameid, int nbframes);
void save_svg(const std::vector<Polygon> &polygons, std::string filename, const std::vector<Vector> *points = NULL, std::string fillcol = "none");
void save_frame(const std::vector<Polygon> &cells, std::string filename, size_t numFluidCells, int frameid = 0);

#endif