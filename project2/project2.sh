#!/bin/bash
for a in static dynamic
do
echo $a
	for b in 1 4096
	do
		for  c in 1 2 4 6 8 10 12 14 16
		do
			g++ -DNUMT=$c -DType=$a -DChunkSize=$b project2.cpp -o project2 -lm -fopenmp
			./project2
		done
	done
done
