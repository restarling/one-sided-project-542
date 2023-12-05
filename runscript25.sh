#!/bin/bash

#SBATCH --ntasks=25

srun -n 25 ./osc 50 >> data.txt;
srun -n 25 ./osc 100 >> data.txt;
srun -n 25 ./osc 150 >> data.txt;
srun -n 25 ./osc 200 >> data.txt;
srun -n 25 ./osc 250 >> data.txt;
srun -n 25 ./osc 300 >> data.txt;
srun -n 25 ./osc 350 >> data.txt;
srun -n 25 ./osc 400 >> data.txt;
