#!/bin/bash

#SBATCH --ntasks=36
#SBATCH --job-name 36procs

srun -n 36 ./osc 1000 >> data.txt;
srun -n 36 ./osc 2000 >> data.txt;
srun -n 36 ./osc 3000 >> data.txt;
srun -n 36 ./osc 4000 >> data.txt;
srun -n 36 ./osc 5000 >> data.txt;
srun -n 36 ./osc 6000 >> data.txt;
srun -n 36 ./osc 7000 >> data.txt;
srun -n 36 ./osc 8000 >> data.txt;
