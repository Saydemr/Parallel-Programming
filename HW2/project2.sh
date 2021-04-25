#!/bin/bash

g++ project2.cpp -o proj -lm -fopenmp

for t in 1 1 2 2 3 3 4 4 
do
    for s in 4 10 25 50 100 250 500 1000
    do
        ./proj $t $s
    done
done