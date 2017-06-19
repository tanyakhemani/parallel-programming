#!/bin/bash
#number of threads:

for t in 8	16	32	64	128	256	512

do
echo Localsize:$t
	#NUMPAD:
	for s in 1000 2000 4000 8000 16000 32000 64000 128000 256000 512000 1024000 2048000 4096000 8192000
	do 
			g++ -DLOCAL_SIZE=$t -DNMB=$s -o MultiplyAdd MultiplyAdd.cpp libOpenCL.so -lm -fopenmp
			./MultiplyAdd
	done   
done
