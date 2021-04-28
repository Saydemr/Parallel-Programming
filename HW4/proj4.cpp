#include <xmmintrin.h>
#include <omp.h>
#include <random>
#include <iostream>


#define SSE_WIDTH		4

int SIZE = 1;
int NUMTRIES = 100;

void SimdMul( float *, float *,float *,int len);
float SimdMulSum( float *a, float *b, int len );
float MulSum( float *a, float *b, int len );
void Mul( float *a, float *b,   float *c,   int len );


float* A = new float[SIZE];
float* B = new float[SIZE];
float* C = new float[SIZE];


int main(int argc, char const *argv[])
{

	#ifndef _OPENMP
    	printf("No OpenMP support!\n" );
    return 1;
    #endif

    if (argc >= 2)
        SIZE = atoi(argv[1]);

	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<> unival(12.5186272,853.019653); // This is my name in Floats - Mid-Little Endian (CDAB) 
    
    printf("SimdMulSum,MulSum,SimdMul,Mul\n");


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

	for (int i = 0; i < NUMTRIES; ++i)
	{
		time0 = omp_get_wtime();
		SimdMulSum(A,B,SIZE);
		time1 = omp_get_wtime();

		if((time1-time0) < SimdMulSum_time)
			SimdMulSum_time = time1 - time0;


		time0 = omp_get_wtime();
		MulSum(A,B,SIZE);
		time1 = omp_get_wtime();

		if((time1-time0) < MulSum_time)
			MulSum_time = time1 - time0;
			

		time0 = omp_get_wtime();
		SimdMul(A,B,C,SIZE);
		time1 = omp_get_wtime();

		if((time1-time0) < SimdMul_time)
			SimdMul_time = time1 - time0;


		time0 = omp_get_wtime();
		Mul(A,B,C,SIZE);
		time1 = omp_get_wtime();

		if((time1-time0) < Mul_time)
			Mul_time = time1 - time0;
	}

	printf("%6.2lf,%6.2lf,%6.2lf,%6.2lf\n",SimdMulSum_time,MulSum_time,SimdMul_time,Mul_time);
	
	return 0;
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



void
Mul( float *a, float *b,   float *c,   int len )
{

	for( int i = 0; i < len; ++i )
	{
		c[i] = a[i] * b[i];
	}
}


float
MulSum( float *a, float *b, int len )
{
	float sum = 0.0f;

	for( int i = 0; i < len; ++i )
	{
		sum += a[i] * b[i];
	}

	return sum;
}