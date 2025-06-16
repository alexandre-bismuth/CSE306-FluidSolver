#ifndef OPTIMAL_TRANSPORT_HPP
#define OPTIMAL_TRANSPORT_HPP

#include "voronoi_diagram.hpp"
#include "lbfgs.h"

class OptimalTransport
{
public:
    OptimalTransport() {};

    void optimize();
    VoronoiDiagram vor;
};

lbfgsfloatval_t evaluate(void *instance, const lbfgsfloatval_t *w, lbfgsfloatval_t *g, const int n, const lbfgsfloatval_t step);
int progress(void *instance, const lbfgsfloatval_t *x, const lbfgsfloatval_t *g, const lbfgsfloatval_t fx,
             const lbfgsfloatval_t xnorm, const lbfgsfloatval_t gnorm, const lbfgsfloatval_t step, int n, int k, int ls);

#endif