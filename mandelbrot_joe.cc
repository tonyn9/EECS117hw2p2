/**
 *  \file mandelbrot_joe.cc
 *
 *  \brief Implement your parallel mandelbrot set in this file.
 */

#include <iostream>
#include <cstdlib>
#include <mpi.h>
#include <string>
#include <math.h>
#include "timer.c"
#include "render.hh"

//make substitutions throughout the file in which it is located
#define WIDTH 1000
#define HEIGHT 1000

using namespace std;

/*
Intern Joe Block implements the above computation with P MPI processes as well. His strategy
is to make process p compute all of the (valid) rows pN, pN + 1, pN + 2,...pN + (N − 1)
where N = bheight/Pc and then use MPI gather to collect all of the values at the root process for
rendering the fractal.
*/


int
mandelbrot(double x, double y)
{
    int maxit = 511; //given in homework hint
    int it = 0;
    double cx = x;
    double cy = y;
    double newx;
    double newy;

    for (it = 0; it < maxit && (x*x + y*y) < 4; ++it)
        {
    newx = x*x - y*y + cx;
    newy = 2*x*y + cy;
    x = newx;
    y = newy;
  }
  return it;
}

}
int
main (int argc, char* argv[])
{
  struct stopwatch_t* timer;
  timer = stopwatch_create ();
  stopwatch_init ();
  stopwatch_start (timer);

  //MPI Initialization
  int rank=0, np=0, namelen=0;
  char hostname[MPI_MAX_PROCESSOR_NAME+1];

  MPI_Init (&argc, &argv);	/* starts MPI */
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);	/* Get process id */
  MPI_Comm_size (MPI_COMM_WORLD, &np);	/* Get number of processes */
  MPI_Get_processor_name (hostname, &namelen); /* Get hostname of node */

  if(rank == 0)
  {
    printf("Mandelbrot Image Using Joe's Logic is Starting Now!\n");
  }

  //Mandelbrot Code
  double minX = -2.1;
  double maxX = 0.7;
  double minY = -1.25;
  double maxY = 1.25;

  int height, width;
  if (argc == 3)
    {
     height = atoi (argv[1]);
     width = atoi (argv[2]);
     assert (height > 0 && width > 0);
    }
  else
    {
     fprintf (stderr, "usage: %s <height> <width>\n", argv[0]);
     fprintf (stderr, "where <height> and <width> are the dimensions of the image.\n");
     return -1;
    }
  //Rounding up value of height and width
  height = round((float)height/(float)np) * np;
  width = round((float)width/(float)np) * np;

  double it = (maxY - minY)/height;
  double jt = (maxX - minX)/width;
  double x, y;


  /* Lucky you, you get to write MPI code */
}

/* eof */
