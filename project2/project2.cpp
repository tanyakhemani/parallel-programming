#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

double array[32768];
int ArraySize=32768;
float
Ranf( float low, float high )
{
	float r = (float) rand();		// 0 - RAND_MAX

	return(   low  +  r * ( high - low ) / (float)RAND_MAX   );
}

int main()
{
int i,j;

for (i=0; i<ArraySize; i++)
{
	array[i]= Ranf (-1.f,1.f);
}

double time0=omp_get_wtime();
omp_set_num_threads(NUMT);
#pragma omp parallel for schedule(Type, ChunkSize) 
for (i=0; i<ArraySize; i++)
{
	double prod=1.;
	for (j=0; j<i; j++)
	{
		prod *= array[j];
	}

}
double time1=omp_get_wtime();

long int numMuled = (long int)ArraySize * (long int)(ArraySize+1) / 2;  // count of how many multiplications were done:
fprintf( stderr, "Threads = %2d; ChunkSize = %5d; MegaMults/sec = %10.2lf\n", NUMT, ChunkSize, (double)numMuled/(time1-time0)/1000000. );

return 0;
}