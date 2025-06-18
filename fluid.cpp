#include "headers/fluid.hpp"

Fluid::Fluid(size_t N, size_t numFluidCells, double mass, double eps, double dt) : N(N), numFluidCells(numFluidCells), mass(mass), eps(eps), dt(dt)
{
    particles.resize(N);
    velocities.resize(N, Vector());
    for (size_t i = 0; i < N; ++i)
        particles[i] = Vector(uniform(engine), uniform(engine));
    ot.vor.points = particles;
    ot.vor.weights.resize(N + 1, 1);
    ot.vor.weights[N] = 0.99;
    ot.vor.compute();
}

void Fluid::stepGallouetMerigot()
{ // Code based on Section 5.4, Algorithm 8 (p. 122)
    Vector g(0, -9.81);
    ot.vor.points = particles;
    ot.optimize();
    for (size_t i = 0; i < particles.size(); ++i)
    {
        Vector FSpring = (ot.vor.cells[i].centroid() - particles[i]) / sqr(eps);
        Vector F = FSpring + mass * g;
        velocities[i] += dt * F / mass;
        particles[i] += dt * velocities[i];
    }
}

void Fluid::simulate()
{
    size_t nbIter = 100;
    for (size_t i = 0; i < nbIter; ++i)
    {
        stepGallouetMerigot();
        ot.vor.points = particles;
        ot.vor.compute();
        std::cout << "Creating frame #" << i + 1 << std::endl;
        save_frame(ot.vor.cells, "output/frames/frame", numFluidCells, i);
    }
}