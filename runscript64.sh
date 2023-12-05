#!/bin/bash

#SBATCH --ntasks=64

srun -n 64 ./osc 50 >> data64.txt;
srun -n 64 ./osc 150 >> data64.txt;
srun -n 64 ./osc 250 >> data64.txt;
srun -n 64 ./osc 350 >> data64.txt;
