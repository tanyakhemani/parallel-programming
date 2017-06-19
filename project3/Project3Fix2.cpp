#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

struct s
{
	float value;
	
	
} Array[4];


	int main( int argc, char *argv[ ] )
{
	
	omp_set_num_threads( NUMT );

	const int SomeBigNumber = 100000000;	// keep < 2B
 
  	double time2= omp_get_wtime();
	#pragma omp parallel for 

	for( int i = 0; i < 4; i++ )
		{
			unsigned int seed = 0;
			float tmp = Array[ i ].value;
			for( unsigned int j = 0; j < SomeBigNumber; j++ )
			{
			tmp = tmp + (float)rand_r(&seed);
			}
			Array[ i ].value = tmp;
		}
	

	double time3= omp_get_wtime();
	double megaMults = SomeBigNumber*4/(time3-time2)/1000000.;

	
	printf( " Performance for Fix2= %8.2lf MegaMults/Sec\n", megaMults );
	return 0;
}
	
