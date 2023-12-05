#!/bin/bash

#SBATCH --ntasks=9

srun -n 9 ./osc 50 >> data.txt;
srun -n 9 ./osc 100 >> data.txt;
srun -n 9 ./osc 150 >> data.txt;
srun -n 9 ./osc 200 >> data.txt;
srun -n 9 ./osc 250 >> data.txt;
srun -n 9 ./osc 300 >> data.txt;
srun -n 9 ./osc 350 >> data.txt;
srun -n 9 ./osc 400 >> data.txt;
