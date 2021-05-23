#!/bin/bash
#SBATCH  -J  MatrixMul
#SBATCH  -A  cs475-575
#SBATCH  -p  class
#SBATCH  --gres=gpu:1
#SBATCH  -o  proj6_out3.csv
#SBATCH  -e  matrixmul3.err
echo ",1024,8192,65536,262144,1048576,2097152,4194304,8388608,"
for t in 32 64 128 256
do
	echo -n $t","
	for s in 1024 8192 65536 262144 1048576 2097152 4194304 8388608
	do
		g++ -DNUM_ELEMENTS=$s -DLOCAL_SIZE=$t -o arrayRed third.cpp /usr/local/apps/cuda/cuda-10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp
		./arrayRed
	done
	echo
done