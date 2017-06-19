#!/bin/bash

for x in 1000 10000 20000 40000 60000 80000 100000 250000 400000 500000 6500000 7000000 9000000 10000000 15000000 26000000 30000000
do 
echo Arraysize:$x
 
	g++ -DARRAYSIZE=$x simd.p51.cpp -o project5 -lm -fopenmp
	./project5
done
