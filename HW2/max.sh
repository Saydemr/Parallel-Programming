#!/bin/bash

g++ project2.cpp -o proj -O3 -lm -fopenmp

for t in 4 
do
    for s in 46340
    do
        ./proj $t $s
    done
done