#!/bin/bash

#SBATCH --ntasks=25
#SBATCH --job-name 25procs

srun -n 25 ./osc 1000 >> data.txt;
srun -n 25 ./osc 2000 >> data.txt;
srun -n 25 ./osc 3000 >> data.txt;
srun -n 25 ./osc 4000 >> data.txt;
srun -n 25 ./osc 5000 >> data.txt;
srun -n 25 ./osc 6000 >> data.txt;
srun -n 25 ./osc 7000 >> data.txt;
srun -n 25 ./osc 8000 >> data.txt;
