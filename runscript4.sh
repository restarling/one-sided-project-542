#!/bin/bash

#SBATCH --ntasks=4
#SBATCH --job-name 4procs

srun -n 4 ./osc 1000 >> data.txt;
srun -n 4 ./osc 2000 >> data.txt;
srun -n 4 ./osc 3000 >> data.txt;
srun -n 4 ./osc 4000 >> data.txt;
srun -n 4 ./osc 5000 >> data.txt;
srun -n 4 ./osc 6000 >> data.txt;
srun -n 4 ./osc 7000 >> data.txt;
srun -n 4 ./osc 8000 >> data.txt;
