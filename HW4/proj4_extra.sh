#!/bin/bash

echo "NUMT, Size, SimdMulSum,  MulSum,    SimdMul,      Mul,, Speedup MulSum, Speedup Mul"

for t in 1 2 4 1 2 4 
do

    for s in 1024 4096 10240 32768 48000 64000 128000 250000 500000 1000000 2000000
    do
    	echo -n $t","$s","
    	g++ -DNUMT=$t -DSIZE=$s proj4_extra.cpp -o proj4_ext -std=c++11 -lm -fopenmp
        ./proj4_ext
    done
done