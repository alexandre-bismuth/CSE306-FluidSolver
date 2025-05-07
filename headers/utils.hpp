#ifndef UTILS_HPP
#define UTILS_HPP

#include <algorithm>
#include <random>

static std::default_random_engine engine;
static std::uniform_real_distribution<double> uniform;

static double sqr(double x) { return x * x; };
static double clamp(double x, double floor, double ceil) { return std::max(std::min(ceil, x), floor); }

#endif