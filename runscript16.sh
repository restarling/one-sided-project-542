#!/bin/bash

#SBATCH --partition=normal
#SBATCH --ntasks=16
#SBATCH --job-name 16procs

srun -n 16 ./osc 1000 >> data.txt;
srun -n 16 ./osc 2000 >> data.txt;
srun -n 16 ./osc 3000 >> data.txt;
srun -n 16 ./osc 4000 >> data.txt;
srun -n 16 ./osc 5000 >> data.txt;
srun -n 16 ./osc 6000 >> data.txt;
srun -n 16 ./osc 7000 >> data.txt;
srun -n 16 ./osc 8000 >> data.txt;
