#!/bin/bash
#number of threads:

for t in 1 2 4 8

do
echo Number of threads: $t
	#NUMPAD:
	for s in 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
	do 

			g++ -DNUMT=$t -DNUMPAD=$s Project3Fix1.cpp -o Project3Fix1 -lm -fopenmp
			./Project3Fix1
	done   
done

