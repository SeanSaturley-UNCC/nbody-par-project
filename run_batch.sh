#!/bin/bash
#SBATCH --job-name=nbody-bench
#SBATCH --output=output/nbody-bench-%j.out
#SBATCH --error=output/nbody-bench-%j.err
#SBATCH --partition=Centaurus
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=4
#SBATCH --time=02:00:00

module load gcc/12.2.0
export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK

# Random 100 particles
./bin/nbody-par 100 1 10000 100 $OMP_NUM_THREADS > output/random-100-par.tsv

# Random 1000 particles
./bin/nbody-par 1000 1 10000 100 $OMP_NUM_THREADS > output/random-1000-par.tsv

# Solar system
./bin/nbody-par planet 200 5000000 10000 $OMP_NUM_THREADS > output/solar-par.tsv
