#!/bin/bash

g++ project1.cpp -o proj1 -lm -fopenmp

for t in 4
do
	for s in 1000000000
	do
		./proj1 $t $s
	done
done