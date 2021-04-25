#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <stdlib.h>

#define XMIN     -1.
#define XMAX      1.
#define YMIN     -1.
#define YMAX      1.

#define N 0.7
#define NUMTRIES 10

int NUMNODES = 1000;
int NUMT = 1;

double Height( int, int );  // function prototype

int main( int argc, char *argv[])
{

    #ifndef _OPENMP
    fprintf("No OpenMP support!\n" );
    return 1;
    #endif

    if (argc >= 2)
        NUMT = atoi(argv[1]);
    if (argc >= 3)
        NUMNODES = atoi(argv[2]);

    omp_set_num_threads( NUMT );

    double volume = 0.;
    double maxPerformance = 0.;

    // the area of a single full-sized tile:
    // (not all tiles are full-sized, though)

    double fullTileArea = (  ( ( XMAX - XMIN )/(double)(NUMNODES-1) )  *
                ( ( YMAX - YMIN )/(double)(NUMNODES-1) )  );

    double halfTile = fullTileArea / 2.0;
    double quarterTile = fullTileArea / 4.0;

    // sum up the weighted heights into the variable "volume"
    // using an OpenMP for loop and a reduction:

    for( int tries = 0; tries < NUMTRIES; ++tries)
    {   

    double time0 = omp_get_wtime();

    #pragma omp parallel for collapse(2) default(none),shared(NUMNODES,fullTileArea,halfTile,quarterTile),reduction(+:volume)

        for( int iv = 0; iv < NUMNODES ; iv++ )
        {
            for( int iu = 0; iu < NUMNODES; iu++ )
            {
                double z = Height( iu, iv );
                double vol = 0.0;   
                if (iv == 0 || iv == NUMNODES-1) 
                {   
                    if (iu == 0 || iu == NUMNODES-1) // quarter 
                    {   
                        vol = z * quarterTile; 
                    }
                    else // half
                    {
                        vol = z * halfTile;
                    }

                }
                else 
                {
                    if (iu == 0 || iu == NUMNODES-1) // half
                    {   
                        vol = z * halfTile; 
                    }
                    else // full
                    {
                        vol = z * fullTileArea;
                    }
                    
                }
                volume += vol;
            }
        }
        double time1 = omp_get_wtime();
        double megaHeightCalculationsPerSecond = (double)NUMNODES * (double)NUMNODES / ( time1 - time0 );
        if( megaHeightCalculationsPerSecond > maxPerformance )
            maxPerformance = megaHeightCalculationsPerSecond;
    }

    double actualVolume = (double)volume * 2.0 / (double)NUMTRIES;
    
    printf("%2d threads : %8d pins ; volume = %6.10lf ; MegaHeights Computed/sec = %6.6lf \n",
            NUMT, NUMNODES*NUMNODES, actualVolume, maxPerformance / 1000000.);

}


double
Height( int iu, int iv )    // iu,iv = 0 .. NUMNODES-1
{
    double x = -1.  +  2.*(double)iu /(double)(NUMNODES-1);    // -1. to +1.
    double y = -1.  +  2.*(double)iv /(double)(NUMNODES-1);    // -1. to +1.

    double xn = pow( fabs(x), (double)N );
    double yn = pow( fabs(y), (double)N );
    double r = 1. - xn - yn;
    if( r <= 0. )
            return 0.;
    double height = pow( r, 1./(double)N );
    return height;
}