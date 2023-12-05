#!/bin/bash

#SBATCH --ntasks=4

srun -n 4 ./osc 50 >> data.txt;
srun -n 4 ./osc 100 >> data.txt;
srun -n 4 ./osc 150 >> data.txt;
srun -n 4 ./osc 200 >> data.txt;
srun -n 4 ./osc 250 >> data.txt;
srun -n 4 ./osc 300 >> data.txt;
srun -n 4 ./osc 350 >> data.txt;
srun -n 4 ./osc 400 >> data.txt;
