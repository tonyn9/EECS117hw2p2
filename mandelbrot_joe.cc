/**
 *  \file mandelbrot_joe.cc
 *
 *  \brief Implement your parallel mandelbrot set in this file.
 */

#include <iostream>
#include <cstdlib>
#include <mpi.h>

#include "timer.c"
#include "render.hh"


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
main (int argc, char* argv[])
{
  /* Lucky you, you get to write MPI code */
}

/* eof */
