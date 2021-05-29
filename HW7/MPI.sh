#!/bin/bash
#SBATCH  -J Heat
#SBATCH  -A cs475-575
#SBATCH  -p class
#SBATCH  -N 8      # number of nodes
#SBATCH  -n 8      # number of tasks
#SBATCH  -o  mpi.txt
#SBATCH  -e  mpi.err
mpic++ MPI.cpp -o mpi -lm
mpiexec -mca btl self,tcp -np  4  mpi