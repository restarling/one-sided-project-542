#!/bin/bash

#SBATCH --ntasks=1

srun -n 1 ./osc 50 >> data1.txt;
srun -n 1 ./osc 150 >> data1.txt;
srun -n 1 ./osc 250 >> data1.txt;
srun -n 1 ./osc 350 >> data1.txt;
