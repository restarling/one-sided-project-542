#!/bin/bash

#SBATCH --partition=normal
#SBATCH --ntasks=49
#SBATCH --job-name 49procs

srun -n 49 ./osc 1000 >> data.txt;
srun -n 49 ./osc 2000 >> data.txt;
srun -n 49 ./osc 3000 >> data.txt;
srun -n 49 ./osc 4000 >> data.txt;
srun -n 49 ./osc 5000 >> data.txt;
srun -n 49 ./osc 6000 >> data.txt;
srun -n 49 ./osc 7000 >> data.txt;
srun -n 49 ./osc 8000 >> data.txt;
