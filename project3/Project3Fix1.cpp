#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

struct s
{
	float value;
	int pad[NUMPAD];
	
} Array[4];


	int main( int argc, char *argv[ ] )
{
	omp_set_num_threads( NUMT );

	const int SomeBigNumber = 100000000;	// keep < 2B
 
  	double time0= omp_get_wtime();
	#pragma omp parallel for 
	for( int i = 0; i < 4; i++ )
	{
		unsigned int seed = 0;		// automatically private
		for( unsigned int j = 0; j < SomeBigNumber; j++ )
		{
			Array[ i ].value = Array[ i ].value + (float)rand_r( &seed );
		}

	}

	double time1= omp_get_wtime();


	
	double megaMults = SomeBigNumber*4/(time1-time0)/1000000.;

	
	printf( " NUMPAD=%d; Performance for Fix1 = %8.2lf MegaMults/Sec\n", NUMPAD, megaMults );
	return 0;
}
	


