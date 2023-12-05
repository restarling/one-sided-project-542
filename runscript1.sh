#!/bin/bash

#SBATCH --ntasks=1

srun -n 1 ./osc 50 >> data.txt;
srun -n 1 ./osc 100 >> data.txt;
srun -n 1 ./osc 150 >> data.txt;
srun -n 1 ./osc 200 >> data.txt;
srun -n 1 ./osc 250 >> data.txt;
srun -n 1 ./osc 250 >> data.txt;
srun -n 1 ./osc 350 >> data.txt;
srun -n 1 ./osc 400 >> data.txt;
