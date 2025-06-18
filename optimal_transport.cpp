#include "headers/optimal_transport.hpp"
#include "headers/utils.hpp"
#include <iostream>

void OptimalTransport::optimize()
{ // Code based on Section 4.4.4 (p. 102-103) from the textbook
    const size_t numWeights = vor.weights.size();
    lbfgsfloatval_t fx;
    std::vector<lbfgsfloatval_t> weights(numWeights, 0);

    lbfgs_parameter_t param;
    lbfgs_parameter_init(&param);
    lbfgs(numWeights, weights.data(), &fx, evaluate, progress, this, &param);
    memcpy(vor.weights.data(), weights.data(), numWeights * sizeof(lbfgsfloatval_t));
}

lbfgsfloatval_t evaluate(void *instance, const lbfgsfloatval_t *x, lbfgsfloatval_t *g,
                         const int n, const lbfgsfloatval_t step)
{
    // Function taken from the libLBFGS library https://github.com/chokkan/liblbfgs/blob/master/sample/sample.c
    // Code based on Section 4.4.4 (p. 102-103) from the textbook
    // Mathematical formulas were slightly rearranged to simplify the code
    OptimalTransport *ot = (OptimalTransport *)instance;
    memcpy(ot->vor.weights.data(), x, n * sizeof(lbfgsfloatval_t));
    ot->vor.compute();
    lbfgsfloatval_t fx = 0.;
    double sum = 0.;
    const size_t N = n - 1;
    for (size_t i = 0; i < N; ++i)
    {
        const double intSquareDist = ot->vor.cells[i].integralNorm2(ot->vor.points[i]);
        sum += ot->vor.cells[i].area();
        g[i] = ot->vor.cells[i].area() - VOL_FLUID / N;
        fx += intSquareDist - x[i] * g[i];
    }
    // Code based on Section 5.4 (p. 123) from the textbook
    double estimate = 1. - sum, desired = 1. - estimate;
    g[N] = desired / N - estimate;
    fx += x[N] * (desired - estimate);
    return -fx;
}

int progress(void *instance, const lbfgsfloatval_t *x, const lbfgsfloatval_t *g,
             const lbfgsfloatval_t fx, const lbfgsfloatval_t xnorm, const lbfgsfloatval_t gnorm,
             const lbfgsfloatval_t step, int n, int k, int ls)
{
    // Function taken from the libLBFGS library https://github.com/chokkan/liblbfgs/blob/master/sample/sample.c
    // Code based on Section 4.4.4 (p. 102-103) from the textbook
    // We leave I/O for debugging but remove it for testing since I/O is slow
    // printf("Iteration %d:\n", k);
    // printf("  fx = %f, x[0] = %f, x[1] = %f\n", fx, x[0], x[1]);
    // printf("  xnorm = %f, gnorm = %f, step = %f\n", xnorm, gnorm, step);
    // printf("\n");
    return 0;
}
