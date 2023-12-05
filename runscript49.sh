#!/bin/bash

#SBATCH --ntasks=49

srun -n 49 ./osc 50 >> data49.txt;
srun -n 49 ./osc 150 >> data49.txt;
srun -n 49 ./osc 250 >> data49.txt;
srun -n 49 ./osc 350 >> data49.txt;
