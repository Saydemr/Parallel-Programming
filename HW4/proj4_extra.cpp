#include <xmmintrin.h>
#include <omp.h>
#include <random>
#include <iostream>

#define SSE_WIDTH		4

int NUMTRIES = 10000;

#ifndef NUMT
#define NUMT             4
#endif


#ifndef SIZE
#define SIZE             1024000
#endif

#define NUM_ELEMENTS_PER_CORE SIZE / NUMT


inline void Mul( float *a, float *b,   float *c,   int len );
inline float MulSum( float *a, float *b, int len );
inline void SimdMul( float *a, float *b,   float *c,   int len );
inline float SimdMulSum( float *a, float *b, int len );


int main(int argc, char const *argv[])
{

	#ifndef _OPENMP
    	printf("No OpenMP support!\n" );
    return 1;
    #endif

	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_real_distribution<float> unival(12.5186272,853.019653); // This is my name in Floats - Mid-Little Endian (CDAB) 


   	float* A = new float[SIZE];
   	float* B = new float[SIZE];
   	float* C = new float[SIZE];


   for (int i = 0; i < SIZE; ++i)
    {
    	B[i] = unival(rng);
    	A[i] = unival(rng);
    }

    double time0 = 0.0;
    double time1 = 0.0;

    double SimdMul_time = 99.9;
    double SimdMulSum_time = 99.9;
    double Mul_time = 99.9;
    double MulSum_time = 99.9;

    omp_set_num_threads( NUMT );

	for (int i = 0; i < NUMTRIES; ++i)
	{
		time0 = omp_get_wtime();
		#pragma omp parallel default(none), shared(A,B)
		{
			int first = omp_get_thread_num() * NUM_ELEMENTS_PER_CORE;
			SimdMulSum(&A[first],&B[first], NUM_ELEMENTS_PER_CORE);	
		}
		
		time1 = omp_get_wtime();

		if((time1-time0) < SimdMulSum_time)
			SimdMulSum_time = time1 - time0;


		time0 = omp_get_wtime();
		MulSum(A,B,SIZE);
		time1 = omp_get_wtime();

		if((time1-time0) < MulSum_time)
			MulSum_time = time1 - time0;


		time0 = omp_get_wtime();
		#pragma omp parallel default(none), shared(A,B,C)
		{
			int first = omp_get_thread_num() * NUM_ELEMENTS_PER_CORE;
			SimdMul(&A[first], &B[first], &C[first], NUM_ELEMENTS_PER_CORE);	
		}
		
		time1 = omp_get_wtime();

		if((time1-time0) < SimdMul_time)
			SimdMul_time = time1 - time0;


		time0 = omp_get_wtime();
		Mul(A,B,C,SIZE);
		time1 = omp_get_wtime();

		if((time1-time0) < Mul_time)
			Mul_time = time1 - time0;
	}

	// calculate the performance
	double p1 = (double)SIZE/ SimdMulSum_time / 1000000.0;
	double p2 = (double)SIZE/ MulSum_time / 1000000.0;
	double p3 = (double)SIZE/ SimdMul_time / 1000000.0;
	double p4 = (double)SIZE/ Mul_time / 1000000.0;

	// commas for .csv file, use "proj4.sh > out.csv" in the terminal
	printf("%10.6lf,%10.6lf,%10.6lf,%10.6lf,\n",p1, p2, p3, p4);

	return 0;
}


void
Mul( float *a, float *b,   float *c,   int len )
{
	omp_set_num_threads( NUMT );
	#pragma omp parallel for default(none), shared(a,b,c,len)
	for( int i = 0; i < len; ++i )
	{
		c[i] = a[i] * b[i];
	}
}


float
MulSum( float *a, float *b, int len )
{
	float sum = 0.0f;
	omp_set_num_threads( NUMT );
	#pragma omp parallel for default(none), shared(a,b,len), reduction(+:sum)
	for( int i = 0; i < len; ++i )
	{
		sum += a[i] * b[i];
	}

	return sum;
}


void
SimdMul( float *a, float *b,   float *c,   int len )
{
	int limit = ( len/SSE_WIDTH ) * SSE_WIDTH;
	register float *pa = a;
	register float *pb = b;
	register float *pc = c;
	for( int i = 0; i < limit; i += SSE_WIDTH )
	{
		_mm_storeu_ps( pc,  _mm_mul_ps( _mm_loadu_ps( pa ), _mm_loadu_ps( pb ) ) );
		pa += SSE_WIDTH;
		pb += SSE_WIDTH;
		pc += SSE_WIDTH;
	}


	for( int i = limit; i < len; ++i )
	{
		c[i] = a[i] * b[i];
	}
}


float
SimdMulSum( float *a, float *b, int len )
{
	float sum[4] = { 0., 0., 0., 0. };
	int limit = ( len/SSE_WIDTH ) * SSE_WIDTH;
	register float *pa = a;
	register float *pb = b;

	__m128 ss = _mm_loadu_ps( &sum[0] );
	for( int i = 0; i < limit; i += SSE_WIDTH )
	{
		ss = _mm_add_ps( ss, _mm_mul_ps( _mm_loadu_ps( pa ), _mm_loadu_ps( pb ) ) );
		pa += SSE_WIDTH;
		pb += SSE_WIDTH;
	}
	_mm_storeu_ps( &sum[0], ss );

	for( int i = limit; i < len; ++i )
	{
		sum[0] += a[i] * b[i];
	}

	return sum[0] + sum[1] + sum[2] + sum[3];
}