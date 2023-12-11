#!/bin/bash

#SBATCH --ntasks=9
#SBATCH --job-name 9procs

srun -n 9 ./osc 1000 >> data.txt;
srun -n 9 ./osc 2000 >> data.txt;
srun -n 9 ./osc 3000 >> data.txt;
srun -n 9 ./osc 4000 >> data.txt;
srun -n 9 ./osc 5000 >> data.txt;
srun -n 9 ./osc 6000 >> data.txt;
srun -n 9 ./osc 7000 >> data.txt;
srun -n 9 ./osc 8000 >> data.txt;
