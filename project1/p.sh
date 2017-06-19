#!/bin/bash

#number of threads:
for t in 1 2 4 8
do
	#number of subdivisions:
	for s in 6 8 12 68 130 518 1015 5187 9437
	do 
    		 g++ -DNUMT=$t -DNUMNODES=$s project1.cpp -o project_1 -lm -fopenmp
    		./project_1
	done   
done
