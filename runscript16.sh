#!/bin/bash

#SBATCH --ntasks=16

srun -n 16 ./osc 100 >> data16.txt;
srun -n 16 ./osc 200 >> data16.txt;
srun -n 16 ./osc 300 >> data16.txt;
srun -n 16 ./osc 400 >> data16.txt;
