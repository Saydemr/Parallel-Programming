#!/bin/bash

echo "Size, SimdMulSum,  MulSum,    SimdMul,      Mul,, Speedup MulSum, Speedup Mul"
for t in 1024 4096 10240 32768 40000 48000 64000 128000 256000 512000 1024000 2048000
do
	echo -n $t","
	g++ -DSIZE=$t project4.cpp -o proj4 -std=c++11 -lm -fopenmp
    ./proj4 $t

done