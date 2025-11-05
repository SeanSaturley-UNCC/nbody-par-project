#ifndef NBODY_FUNCTIONS_HPP
#define NBODY_FUNCTIONS_HPP

#include <vector>
#include <string>
#include <cstddef>

// gravitational constant (defined in nbody_functions.cpp)
extern double G;

struct simulation {
    size_t nbpart;
    std::vector<double> mass;
    std::vector<double> x, y, z;
    std::vector<double> vx, vy, vz;
    std::vector<double> fx, fy, fz;

    simulation(size_t nb = 0);
};

// initialization
void random_init(simulation& s);
void init_solar(simulation& s);
void load_from_file(simulation& s, const std::string& filename);

// basic ops (shared by sequential & parallel mains)
void reset_force(simulation& s);
void apply_force(simulation& s, size_t i, double dt);
void update_position(simulation& s, size_t i, double dt);

// output
void dump_state(const simulation& s);

#endif // NBODY_FUNCTIONS_HPP
