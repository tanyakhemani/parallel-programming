#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>


//Global variables:
int	    NowYear;		// 2017 - 2022
int	    NowMonth;		// 0 - 11
float	NowPrecip;	    // inches of rain per month
float	NowTemp;		// temperature this month
float	NowHeight;		// grain height in inches
int     NowNumDeer;		// number of deer in the current population
float   EffectOnHeight;
int 	PesticidesUsed;
float	tmpHeight;


// Units of grain growth are inches.
const float GRAIN_GROWS_PER_MONTH 	=		8.0;
const float ONE_DEER_EATS_PER_MONTH =	    0.5;

// Units of precipitation are inches.
const float AVG_PRECIP_PER_MONTH 	=		6.0;	// average
const float AMP_PRECIP_PER_MONTH 	=		6.0;	// plus or minus
const float RANDOM_PRECIP 			=		2.0;	// plus or minus noise

// Units of temperature are degrees Fahrenheit
const float AVG_TEMP 				=	    50.0;	// average
const float AMP_TEMP 				=		20.0;	// plus or minus
const float RANDOM_TEMP 			=		10.0;	// plus or minus noise

const float MIDTEMP 				=		40.0;
const float MIDPRECIP 				=		10.0;


float
SQR(float x)
{
        return x*x;
}

float Ranf (unsigned int *seedp, float low, float high)

{
        float r = (float) rand_r( seedp );              // 0 - RAND_MAX

        return (low + r * (high - low) / (float)RAND_MAX);
}

unsigned int seed;  // a thread-private variable


int RandomPesticideQuantity() //Percentage of pesticides used
{
	return rand() % 101;
}

int Pesticides()
{
	PesticidesUsed=0;
	EffectOnHeight=0;
	PesticidesUsed=RandomPesticideQuantity();
	
	if (PesticidesUsed>50)
	{
		int excesspesticides=PesticidesUsed-50;
		for(int i=0;i<excesspesticides;i++)//reduce height by half inch;
		{
			EffectOnHeight+=0.05; //reduce heigh by 0.05 inch for every pesticide % used
		}
	}
	#pragma omp barrier
	

	#pragma omp barrier

	//do nothing
	#pragma omp barrier
}




int GrainDeer()
{
	//Carrying Capacity of graindeer = Inches height of grain

	
	int tmpNumDeer=NowNumDeer;
	if (tmpNumDeer > NowHeight)
		tmpNumDeer--;

	else

		tmpNumDeer++;

	// DoneComputing barrier:
	#pragma omp barrier

	//Write to global NowNumDeer 
	NowNumDeer = tmpNumDeer;

	// DoneAssigning barrier:
	#pragma omp barrier

	//do nothing
	#pragma omp barrier
	 
}

float Grain()

{

	float tempFactor 	= exp(-SQR(( NowTemp - MIDTEMP)/10.));
	float precipFactor 	= exp(-SQR(( NowPrecip - MIDPRECIP)/10.));

	tmpHeight += tempFactor * precipFactor * GRAIN_GROWS_PER_MONTH;
 	tmpHeight -= (float)NowNumDeer * ONE_DEER_EATS_PER_MONTH;
	tmpHeight-=EffectOnHeight;

 	if (tmpHeight < 0.)
 		tmpHeight = 0.;

	// DoneComputing barrier:
	#pragma omp barrier
 	NowHeight=tmpHeight;
 	
 	// DoneAssigning barrier:
	#pragma omp barrier

	//do nothing 
	#pragma omp barrier
}

/*Watcher
				
	Keeps running until the year is 2023				
	Print current set of global state variables				
	Increment month counter				
	Use new month counter to calculate Temperature and Precipitation				
*/

int watcher()
{
	#pragma omp barrier
	//do nothing

	#pragma omp barrier
	//do nothing

	// Print global variables	
	printf("==============================================================\n");
	printf("Now Year 	= %d\n",   NowYear);			
	printf("Now Month 	= %d\n",   NowMonth);		
	printf("Now NumDeer = %d\n",   NowNumDeer);		
	printf("Now Precip 	= %f\n",   NowPrecip* 2.54);			
	printf("Now Temp 	= %f\n",   (5./9.)*(NowTemp-32));			
	printf("Now Height 	= %f\n",   NowHeight* 2.54);
	printf("Pesticides used = %f\n",   PesticidesUsed * 0.10);
	
	// Update month count and re-compute temperature and precipitation
	NowMonth++;
  	if (NowMonth % 12 == 0)
  		NowYear++;

  	float ang = (30.*(float)NowMonth + 15.) * (M_PI / 180.);
	
	float temp = AVG_TEMP - AMP_TEMP * cos( ang );
	
	NowTemp = temp + Ranf( &seed, -RANDOM_TEMP, RANDOM_TEMP );

	float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
	NowPrecip = precip + Ranf(&seed,  -RANDOM_PRECIP, RANDOM_PRECIP);

   	if (NowPrecip < 0.)
	    NowPrecip = 0.;

	// DonePrinting barrier:
	#pragma omp barrier
}


int main(int argc, char const *argv[])

{
	
	float ang = (30.*(float)NowMonth + 15.) * (M_PI / 180.);
	
	float temp = AVG_TEMP - AMP_TEMP * cos( ang );
	seed = 0;
	NowTemp = temp + Ranf( &seed, -RANDOM_TEMP, RANDOM_TEMP );

	float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
	NowPrecip = precip + Ranf(&seed,  -RANDOM_PRECIP, RANDOM_PRECIP);

   	if ( NowPrecip < 0. )
	     NowPrecip = 0.;

	omp_set_num_threads (4);	// same as # of sections

	// starting date and time:
	NowMonth 	=    0;
	NowYear  	= 2017;

	// starting state (feel free to change this if you want):

	
	NowNumDeer 	= 1;
	NowHeight 	= 1.;

	while (NowYear < 2023)
  	{
		#pragma omp parallel sections
 		{
			#pragma omp section
			{
				GrainDeer( );
			}

			#pragma omp section
			{
				Grain( );
			}

			#pragma omp section
			{
				watcher( );
			}

			#pragma omp section
			{
				Pesticides();
			}
		
 		} 
 	}
	
	return 0;

}

