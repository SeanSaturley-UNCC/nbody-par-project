#include "nbody_functions.hpp"
#include <random>
#include <fstream>
#include <iostream>
#include <cmath>
#include <stdexcept>

double G = 6.674e-11; 

simulation::simulation(size_t nb)
  : nbpart(nb), mass(nb),
    // postition
    x(nb), y(nb), z(nb),
    // velocity
    vx(nb), vy(nb), vz(nb),
    // force
    fx(nb), fy(nb), fz(nb)
{}

void random_init(simulation& s) {
    std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<double> dismass(0.9, 1.0);
    std::normal_distribution<double> dispos(0.0, 1.0);

    for (size_t i = 0; i < s.nbpart; ++i) {
        s.mass[i] = dismass(gen);
      
        s.x[i] = dispos(gen);
        s.y[i] = dispos(gen);
        s.z[i] = 0.0;
      
        s.vx[i] = s.y[i] * 1.5;
        s.vy[i] = -s.x[i] * 1.5;
        s.vz[i] = 0.0;
    }
}

void init_solar(simulation& s) {
    enum Planets {SUN, MERCURY, VENUS, EARTH, MARS, JUPITER, SATURN, URANUS, NEPTUNE, MOON};
    s = simulation(10);

    // Masses in kg
    s.mass[SUN]     = 1.9891e30;
    s.mass[MERCURY] = 3.285e23;
    s.mass[VENUS]   = 4.867e24;
    s.mass[EARTH]   = 5.972e24;
    s.mass[MARS]    = 6.39e23;
    s.mass[JUPITER] = 1.898e27;
    s.mass[SATURN]  = 5.683e26;
    s.mass[URANUS]  = 8.681e25;
    s.mass[NEPTUNE] = 1.024e26;
    s.mass[MOON]    = 7.342e22;

    // Positions (in meters) and velocities (in m/s)
    double AU = 1.496e11;
    s.x = {0, 0.39*AU, 0.72*AU, 1.0*AU, 1.52*AU, 5.20*AU, 9.58*AU, 19.22*AU, 30.05*AU, 1.0*AU + 3.844e8};
    s.y.assign(10, 0.0);
    s.z.assign(10, 0.0);

    s.vx.assign(10, 0.0);
    s.vy = {0, 47870, 35020, 29780, 24130, 13070, 9680, 6800, 5430, 29780 + 1022};
    s.vz.assign(10, 0.0);
}

void load_from_file(simulation& s, const std::string& filename) {
    std::ifstream in(filename);
    if (!in) throw std::runtime_error("cannot open file: " + filename);
    size_t nbpart;
    in >> nbpart;
    s = simulation(nbpart);
    for (size_t i = 0; i < s.nbpart; ++i) {
        in >> s.mass[i];
        in >> s.x[i] >> s.y[i] >> s.z[i];
        in >> s.vx[i] >> s.vy[i] >> s.vz[i];
        in >> s.fx[i] >> s.fy[i] >> s.fz[i];
    }
    if (!in.good() && !in.eof()) throw std::runtime_error("read error");
}

// ---------- Basic operations ----------
void reset_force(simulation& s) {
    for (size_t i = 0; i < s.nbpart; ++i) {
        s.fx[i] = 0.0;
        s.fy[i] = 0.0;
        s.fz[i] = 0.0;
    }
}

void apply_force(simulation& s, size_t i, double dt) {
    s.vx[i] += (s.fx[i] / s.mass[i]) * dt;
    s.vy[i] += (s.fy[i] / s.mass[i]) * dt;
    s.vz[i] += (s.fz[i] / s.mass[i]) * dt;
}

void update_position(simulation& s, size_t i, double dt) {
    s.x[i] += s.vx[i] * dt;
    s.y[i] += s.vy[i] * dt;
    s.z[i] += s.vz[i] * dt;
}



void dump_state(const simulation& s) {
    std::cout << s.nbpart << '\t';
    for (size_t i = 0; i < s.nbpart; ++i) {
        std::cout
            << s.mass[i] << '\t'
            << s.x[i] << '\t' << s.y[i] << '\t' << s.z[i] << '\t'
            << s.vx[i] << '\t' << s.vy[i] << '\t' << s.vz[i] << '\t'
            << s.fx[i] << '\t' << s.fy[i] << '\t' << s.fz[i] << '\t';
    }
    std::cout << '\n';
}
