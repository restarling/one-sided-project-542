#!/bin/bash

#SBATCH --ntasks=4

srun -n 4 ./osc 50 >> data4.txt;
srun -n 4 ./osc 150 >> data4.txt;
srun -n 4 ./osc 250 >> data4.txt;
srun -n 4 ./osc 350 >> data4.txt;
