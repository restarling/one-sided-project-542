#!/bin/bash

#SBATCH --partition=normal
#SBATCH --ntasks=64
#SBATCH --job-name 64procs

srun -n 64 ./osc 1000 >> data.txt;
srun -n 64 ./osc 2000 >> data.txt;
srun -n 64 ./osc 3000 >> data.txt;
srun -n 64 ./osc 4000 >> data.txt;
srun -n 64 ./osc 5000 >> data.txt;
srun -n 64 ./osc 6000 >> data.txt;
srun -n 64 ./osc 7000 >> data.txt;
srun -n 64 ./osc 8000 >> data.txt;
