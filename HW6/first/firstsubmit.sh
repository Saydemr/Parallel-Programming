#!/bin/bash
#SBATCH  -J  MatrixMul
#SBATCH  -A  cs475-575
#SBATCH  -p  class
#SBATCH  --gres=gpu:1
#SBATCH  -o  proj6_out1.csv
#SBATCH  -e  matrixmul1.err
echo ",1024,8192,65536,262144,1048576,2097152,4194304,8388608,"
for t in 8 16 32 64 128 256 512
do
	echo -n $t","
	for s in 1024 8192 65536 262144 1048576 2097152 4194304 8388608
	do
		g++ -DNUM_ELEMENTS=$s -DLOCAL_SIZE=$t -o arrayMul first.cpp /usr/local/apps/cuda/cuda-10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp
		./arrayMul
	done
	echo
done