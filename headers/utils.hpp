#ifndef UTILS_HPP
#define UTILS_HPP

#include <algorithm>
#include <random>
#include "polygon.hpp"
#include "vector.hpp"

static std::default_random_engine engine;
static std::uniform_real_distribution<double> uniform;

inline double sqr(double x) { return x * x; };
inline double clamp(double x, double floor, double ceil) { return std::max(std::min(ceil, x), floor); }

void save_svg_animated(const std::vector<Polygon> &polygons, std::string filename, int frameid, int nbframes);
void save_svg(const std::vector<Polygon> &polygons, std::string filename, const std::vector<Vector> *points = NULL, std::string fillcol = "none");
void write_obj(const char *filename);

#endif