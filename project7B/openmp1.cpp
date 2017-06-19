#include <stdio.h>
#include <fstream>
#include <iostream>
#include <omp.h>

#define SIZE 32768
float Array[2*SIZE];
float  Sums[1*SIZE];



int main(int argc, char const *argv[])
{
	omp_set_num_threads(0);
	FILE *fp = fopen( "signal.txt", "r" );
	if( fp == NULL )
	{
		fprintf( stderr, "Cannot open file 'signal.txt'\n" );
	}

	int Size;
	fscanf( fp, "%d", &Size );
	Size = SIZE;

	for( int i = 0; i < Size; i++ )
	{
		fscanf( fp, "%f", &Array[i] );
		Array[i+Size] = Array[i];		// duplicate the array
	}
	fclose( fp );

	int shift;
	

	double time0=omp_get_wtime();
	#pragma omp parallel for private (shift)
	for( shift = 0; shift < Size; shift++ )
	{
		float sum = 0.;
		for( int i = 0; i < Size; i++ )
		{
			sum += Array[i] * Array[i + shift];
		}
		Sums[shift] = sum;	// note the "fix #2" from false sharing if you are using OpenMP
		
		//printf("%f\n",Sums[shift]);
	}
	double time1=omp_get_wtime();
	
	
	//output signal autocorrelation till 512
	for (shift = 0; shift < 512 ; shift++)
	{
			printf("%f\n",Sums[shift]); 
	}
	printf("Performance:%lf\n", (double)(Size*Size)/(time1-time0)/1000000.);

return 0;
}
