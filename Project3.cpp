#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>


//Declaration of variables.
int	NowYear;		// 2022 - 2027
int	NowMonth;		// 0 - 11

float	NowPrecip;		// inches of rain per month
float	NowTemp;		// temperature this month
float	NowHeight;		// grain height in inches
int	NowNumDeer;		// number of deer in the current population
int     Agent_count;           // count of Agent

const float GRAIN_GROWS_PER_MONTH =		9.0;
const float ONE_DEER_EATS_PER_MONTH =		1.0;

const float AVG_PRECIP_PER_MONTH =		7.0;	// average
const float AMP_PRECIP_PER_MONTH =		6.0;	// plus or minus
const float RANDOM_PRECIP =			2.0;	// plus or minus noise

const float AVG_TEMP =				60.0;	// average
const float AMP_TEMP =				20.0;	// plus or minus
const float RANDOM_TEMP =			10.0;	// plus or minus noise

const float MIDTEMP =				40.0;
const float MIDPRECIP =				10.0;

unsigned int seed = 0;

float Ranf(unsigned int*,float,float);
int Ranf(unsigned int*,int,int);
void computeWeather();
void print();
float SQR( float );
void Deer();
void Grain();
void Watcher();
void MyAgent();



// The main Program
int main( int argc, char *argv[ ] )
{
        #ifndef _OPENMP
                fprintf( stderr, "No OpenMP support!\n" );
                return 1;
        #endif

// starting date and time:
        NowMonth =    0;
        NowYear  = 2022;

// starting state (feel free to change this if you want):
        NowNumDeer = 20;
        NowHeight =  2.;
        Agent_count = 20;
        omp_set_num_threads( 4 );    // set the number of threads to use same as # of sections.
        #pragma omp parallel sections
        {
                #pragma omp section
                {
                        Deer();
                }
                #pragma omp section
                {
                        Grain();
                }
                #pragma omp section
                {
                        Watcher();
                }
                #pragma omp section
                {
                        MyAgent();
                }
        }
}

float Ranf( unsigned int *seedp,  float low, float high )
{
        float r = (float) rand_r( seedp );              // 0 - RAND_MAX

        return(   low  +  r * ( high - low ) / (float)RAND_MAX   );
}


int Ranf( unsigned int *seedp, int ilow, int ihigh )
{
        float low = (float)ilow;
        float high = (float)ihigh + 0.9999f;

        return (int)(  Ranf(seedp, low,high) );
}

//squaring function.
float
SQR( float x )
{
        return x*x;
}

//Simulation function for Deer.
void Deer()
{
        while( NowYear < 2028 )
        {
                int CarryingCapacity =  NowHeight;

                int nextNumDeer = NowNumDeer;
                if ( NowMonth%4 == 0 || NowMonth%6==0 )
                {
                        nextNumDeer += Agent_count;
                }
                if ( nextNumDeer > CarryingCapacity )
                        nextNumDeer--;
                else if ( nextNumDeer < CarryingCapacity )
                        nextNumDeer++;

                //In case the deer count becomes less than 0, then make the count as 0.
                if (nextNumDeer < 0 )
                        nextNumDeer = 0;

                //Done Computing barrier.
                #pragma omp barrier
                NowNumDeer = nextNumDeer;

                //Done Assigning barrier.
                #pragma omp barrier

                //Done Printing barrier.
                #pragma omp barrier     
        }
}

//Simulation function for Grain.
void Grain()
{
        while( NowYear < 2028 )
        {
                float nextHeight = NowHeight;
                float tempFactor = exp(   -SQR(  ( NowTemp - MIDTEMP ) / 10.  )   );
                float precipFactor = exp(   -SQR(  ( NowPrecip - MIDPRECIP ) / 10.  )   );

                nextHeight += tempFactor * precipFactor * GRAIN_GROWS_PER_MONTH;
                nextHeight -= (float)NowNumDeer * ONE_DEER_EATS_PER_MONTH;

                //In case the height of the grain becomes less than 0, make it as 0.
                if( nextHeight < 0. )
                        nextHeight = 0.;

                //Done Computing barrier.
                #pragma omp barrier
                
                NowHeight = nextHeight;

                //Done Assigning barrier.
                #pragma omp barrier

                //Done Printin barrier.
                #pragma omp barrier      
        }
}

//Simulation function for Watcher.
void Watcher()
{
        while( NowYear < 2028 )
        {
                //Done Computing barrier.
                #pragma omp barrier

                //Done Assigning barrier.
                #pragma omp barrier
				print();

                // Increment month and year
                if (NowMonth == 11)
                {
                        NowMonth = 0;
                        NowYear++;
                }
                else
                        NowMonth++;

                //caculate the new precipitation and temparature values.
               computeWeather();
                #pragma omp barrier      
        }
}

//Simulation function for Watcher.
void MyAgent()
{
    while(NowYear < 2028)
	{
        int temp = Agent_count;

        if(Agent_count  > (NowNumDeer/10))
				 temp--;
        else if(Agent_count  <= (NowNumDeer/10)) 
                 temp++;
        
        if(temp < 0){
            temp = 0;
        }

        //Done Computing barrier.
        #pragma omp barrier

        Agent_count = temp;

        //Done Assigning barrier.
        #pragma omp barrier


        //Done Printing barrier.
        #pragma omp barrier
    }
}

void print(){
	
	
                printf("%d,%lf,%lf,%lf,%d,%d\n", NowMonth+1, (NowPrecip * 2.54), (5./9.)*(NowTemp - 32), (NowHeight * 2.54), NowNumDeer, Agent_count);

}

void computeWeather(){
	
	 float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );

                
                float temp = AVG_TEMP - AMP_TEMP * cos( ang );
                NowTemp = temp + Ranf( &seed, -RANDOM_TEMP, RANDOM_TEMP );

                
                float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
                NowPrecip = precip + Ranf( &seed,  -RANDOM_PRECIP, RANDOM_PRECIP );
                if( NowPrecip < 0. )
	                NowPrecip = 0.;

}