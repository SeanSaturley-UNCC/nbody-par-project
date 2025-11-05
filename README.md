# Parallel N-Body Simulation

## Overview
This project simulates an N-body system using C++ and OpenMP.  
It calculates gravitational forces between particles and updates their positions and velocities over time.  
Parallelization uses the provided OmpLoop::parfor().

## Files
- `src/` : Source code
- `Makefile` : Build instructions
- `output/` : Simulation outputs
- `bin/` : Executables
- `run_batch.sh` : Batch script for Centaurus

## Compile
bash
make

## Run Sequential:
./bin/nbody <input> <dt> <nbstep> <printevery>
# Example:
./bin/nbody 100 1 10000 10 > output/random-100-seq.tsv

## Run Parallel:
./bin/nbody-par <input> <dt> <nbstep> <printevery> <nbthreads>
# Example:
./bin/nbody-par 100 1 10000 10 10 > output/random-100-par.tsv

## Batch
sbatch run_batch.sh

## Output Format

- Number of particles
- For each particle: mass, position (x, y, z), velocity (vx, vy, vz), force (fx, fy, fz)
- Values separated by tabs (.tsv)
