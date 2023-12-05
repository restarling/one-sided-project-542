#!/bin/bash

#SBATCH --ntasks=16

srun -n 16 ./osc 50 >> data.txt;
srun -n 16 ./osc 100 >> data.txt;
srun -n 16 ./osc 150 >> data.txt;
srun -n 16 ./osc 200 >> data.txt;
srun -n 16 ./osc 250 >> data.txt;
srun -n 16 ./osc 300 >> data.txt;
srun -n 16 ./osc 350 >> data.txt;
srun -n 16 ./osc 400 >> data.txt;
