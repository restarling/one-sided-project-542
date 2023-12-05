#!/bin/bash

#SBATCH --ntasks=9

srun -n 9 ./osc 50 >> data9.txt;
srun -n 9 ./osc 150 >> data9.txt;
srun -n 9 ./osc 250 >> data9.txt;
srun -n 9 ./osc 350 >> data9.txt;
