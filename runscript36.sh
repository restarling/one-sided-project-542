#!/bin/bash

#SBATCH --ntasks=36

srun -n 36 ./osc 50 >> data.txt;
srun -n 36 ./osc 100 >> data.txt;
srun -n 36 ./osc 150 >> data.txt;
srun -n 36 ./osc 200 >> data.txt;
srun -n 36 ./osc 250 >> data.txt;
srun -n 36 ./osc 300 >> data.txt;
srun -n 36 ./osc 350 >> data.txt;
srun -n 36 ./osc 400 >> data.txt;
