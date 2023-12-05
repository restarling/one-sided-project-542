#!/bin/bash

#SBATCH --ntasks=49

srun -n 49 ./osc 50 >> data.txt;
srun -n 49 ./osc 100 >> data.txt;
srun -n 49 ./osc 150 >> data.txt;
srun -n 49 ./osc 200 >> data.txt;
srun -n 49 ./osc 250 >> data.txt;
srun -n 49 ./osc 300 >> data.txt;
srun -n 49 ./osc 350 >> data.txt;
srun -n 49 ./osc 400 >> data.txt;
