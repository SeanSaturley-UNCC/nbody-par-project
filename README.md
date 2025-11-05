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
```bash
make
