#!/bin/bash



echo "NUMT, Size, SimdMulSum,  MulSum,    SimdMul,      Mul,, Speedup MulSum, Speedup Mul"

for t in 1 1 2 2 3 3 4 4 
do

    for s in 1024 10240 16000 32000 64000 128000 256000 512000 1000000 2000000 8000000
    do
    	echo -n $t","$s","
    	g++ -DNUMT=$t -DSIZE=$t proj4_extra.cpp -o proj4_ext -std=c++11 -lm -fopenmp
        ./proj4_ext
    done
done