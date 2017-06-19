#!/bin/bash
#number of threads:

for t in 1 2 4 8

do
echo Number of threads: $t

			g++ -DNUMT=$t -DNUMPAD=$s Project3Fix2.cpp -o Project3Fix2 -lm -fopenmp

./Project3Fix2

	done   


