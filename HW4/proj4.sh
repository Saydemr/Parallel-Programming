#!/bin/bash

echo "Size, SimdMulSum,  MulSum,    SimdMul,      Mul,, Speedup MulSum, Speedup Mul"
for t in 1024 4096 10240 32768 48000 64000 128000 250000 500000 1000000 2000000
do
	echo -n $t","
	g++ -DSIZE=$t project4.cpp -o proj4 -std=c++11 -lm -fopenmp
    ./proj4 $t

done