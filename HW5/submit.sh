#!/bin/bash
#SBATCH  -J  MatrixMul
#SBATCH  -A  cs475-575
#SBATCH  -p  class
#SBATCH  --gres=gpu:1
#SBATCH  -o  matrixmul.out
#SBATCH  -e  matrixmul.err
echo ",2048,4096,8192,16384,32768,65536,131072,262144,524288,1024000," 
for t in 16 32 64 128
do
    echo -n $t","
    for s in 2048 4096 8192 16384 32768 65536 131072 262144 524288 1024000
    do
        /usr/local/apps/cuda/cuda-10.1/bin/nvcc -DNUMTRIALS=$s -DBLOCKSIZE=$t -o matrixMul project5.cu
        ./matrixMul
    done
    echo
done