#!/bin/bash

#SBATCH --ntasks=25

srun -n 25 ./osc 100 >> data25.txt;
srun -n 25 ./osc 200 >> data25.txt;
srun -n 25 ./osc 300 >> data25.txt;
srun -n 25 ./osc 400 >> data25.txt;
