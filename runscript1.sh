#!/bin/bash

#SBATCH --partition=normal
#SBATCH --ntasks=1
#SBATCH --job-name 1procs

srun -n 1 ./osc 1000 >> data.txt;
srun -n 1 ./osc 2000 >> data.txt;
srun -n 1 ./osc 3000 >> data.txt;
srun -n 1 ./osc 4000 >> data.txt;
srun -n 1 ./osc 5000 >> data.txt;
srun -n 1 ./osc 6000 >> data.txt;
srun -n 1 ./osc 7000 >> data.txt;
srun -n 1 ./osc 8000 >> data.txt;
