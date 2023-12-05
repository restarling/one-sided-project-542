#!/bin/bash

#SBATCH --ntasks=36

srun -n 36 ./osc 50 >> data36.txt;
srun -n 36 ./osc 150 >> data36.txt;
srun -n 36 ./osc 250 >> data36.txt;
srun -n 36 ./osc 350 >> data36.txt;
