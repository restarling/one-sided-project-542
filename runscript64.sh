#!/bin/bash

#SBATCH --ntasks=64

srun -n 64 ./osc 50 >> data.txt;
srun -n 64 ./osc 100 >> data.txt;
srun -n 64 ./osc 150 >> data.txt;
srun -n 64 ./osc 200 >> data.txt;
srun -n 64 ./osc 250 >> data.txt;
srun -n 64 ./osc 300 >> data.txt;
srun -n 64 ./osc 350 >> data.txt;
srun -n 64 ./osc 400 >> data.txt;
