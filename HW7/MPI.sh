#!/bin/bash
#SBATCH  -J Heat
#SBATCH  -A cs475-575
#SBATCH  -p class
#SBATCH  -N 8      # number of nodes
#SBATCH  -n 8      # number of tasks
#SBATCH  -o  mpi.txt
#SBATCH  -e  mpi.err
mpic++ MPI.cpp -o mpi -lm

for t in 4 8 16 32
do 
	mpiexec -mca btl self,tcp -np  $t  mpi
done