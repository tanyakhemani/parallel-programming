#include "simd.p5.h"
#include <stdio.h>
#include <iostream>
#include <fstream>

#define SIZE 32768
float Array[2*SIZE];
float  Sums[1*SIZE];

float
SimdMulSum( float *a, float *b, int len )
{
	float sum[4] = { 0., 0., 0., 0. };
	int limit = ( len/SSE_WIDTH ) * SSE_WIDTH;

	__asm
	(
		".att_syntax\n\t"
		"movq    -40(%rbp), %rbx\n\t"		// a
		"movq    -48(%rbp), %rcx\n\t"		// b
		"leaq    -32(%rbp), %rdx\n\t"		// &sum[0]
		"movups	 (%rdx), %xmm2\n\t"		// 4 copies of 0. in xmm2
	);

	for( int i = 0; i < limit; i += SSE_WIDTH )
	{
		__asm
		(
			".att_syntax\n\t"
			"movups	(%rbx), %xmm0\n\t"	// load the first sse register
			"movups	(%rcx), %xmm1\n\t"	// load the second sse register
			"mulps	%xmm1, %xmm0\n\t"	// do the multiply
			"addps	%xmm0, %xmm2\n\t"	// do the add
			"addq $16, %rbx\n\t"
			"addq $16, %rcx\n\t"
		);
	}

	__asm
	(
		".att_syntax\n\t"
		"movups	 %xmm2, (%rdx)\n\t"	// copy the sums back to sum[ ]
	);

	for( int i = limit; i < len; i++ )
	{
		sum[i-limit] += a[i] * b[i];
	}

	return sum[0] + sum[1] + sum[2] + sum[3];
}

int main(int argc, char const *argv[])
{
	
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
	for( shift = 0; shift < Size; shift++ )
	{
		float sum = 0.;
		Sums[shift] = SimdMulSum( &Array[0], &Array[0+shift], Size );
		//printf("%f\n",Sums[shift]);
	}
	double time1=omp_get_wtime();
	
	
	for( shift = 0; shift < 512; shift++ )
	{
	printf("%f\n",Sums[shift] );
	}
	printf("Performance:%lf\n", (double)(Size*Size)/(time1-time0)/1000000.);
return 0;
}
