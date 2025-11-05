#include "nbody_functions.hpp"
#include "omp_loop.hpp"

#include <cstdlib>
#include <iostream>
#include <string>
#include <cmath>

// sequential force computation
void compute_forces_parallel(simulation& s, double softening) {
   
    // reset forces first (
    OmpLoop::parfor(0, s.nbpart, [&](std::size_t i){
        s.fx[i] = s.fy[i] = s.fz[i] = 0.0;
    });

    // compute the total force acting on i for each particle i
    OmpLoop::parfor(0, s.nbpart, [&](std::size_t i){
        double xi = s.x[i], yi = s.y[i], zi = s.z[i];
        double mi = s.mass[i];
        double fx = 0.0, fy = 0.0, fz = 0.0;
        for (std::size_t j = 0; j < s.nbpart; ++j) {
            if (j == i) continue;
            double dx = s.x[j] - xi;
            double dy = s.y[j] - yi;
            double dz = s.z[j] - zi;
            double dist_sq = dx*dx + dy*dy + dz*dz + softening;
            double inv_dist = 1.0 / std::sqrt(dist_sq);
            double inv_dist3 = inv_dist * inv_dist * inv_dist;
            double f = G * mi * s.mass[j] * inv_dist3;
            fx += dx * f;
            fy += dy * f;
            fz += dz * f;
        }
        s.fx[i] = fx;
        s.fy[i] = fy;
        s.fz[i] = fz;
    });
}

int main(int argc, char* argv[]) {
    // usage: <input> <dt> <nbstep> <printevery> and now <nbthreads>
    if (argc != 6) {
        std::cerr << "usage: " << argv[0] << " <input> <dt> <nbstep> <printevery> <nbthreads>\n";
        std::cerr << "input: number (random), 'planet', or filename\n";
        return -1;
    }

    std::string input = argv[1];
    double dt = std::atof(argv[2]);
    size_t nbstep = std::stoul(argv[3]);
    size_t printevery = std::stoul(argv[4]);

    // new 
    int nbthreads = std::atoi(argv[5]);

    simulation s(1);
    // init
    char* endptr = nullptr;
    long parsed = std::strtol(argv[1], &endptr, 10);
    if (endptr != argv[1] && parsed > 0) {
        s = simulation(static_cast<size_t>(parsed));
        random_init(s);
    } else {
        if (input == "planet") init_solar(s);
        else load_from_file(s, input);
    }

    // configure omploop wrapper
    OmpLoop::setNbThread(nbthreads);
    OmpLoop::setGranularity(1);

    const double softening = 1e-9;

    for (size_t step = 0; step < nbstep; ++step) {
        if (step % printevery == 0) dump_state(s);
        compute_forces_parallel(s, softening);
        
        // integrate (apply forces then update positions)
        OmpLoop::parfor(0, s.nbpart, [&](std::size_t i){
            apply_force(s, i, dt);
        });
       
        OmpLoop::parfor(0, s.nbpart, [&](std::size_t i){
            update_position(s, i, dt);
        });
    }

    
    return 0;
}
