/**
 *  \file mandelbrot_joe.cc
 *
 *  \brief Implement your parallel mandelbrot set in this file.
 */

 //https://piazza-resources.s3.amazonaws.com/ixjucpwhwnp4i7/iyq2tknn3s04aj/mpi.pdf?AWSAccessKeyId=AKIAIEDNRLJ4AZKBW6HA&Expires=1487553270&Signature=XXEzyHKB3uNgNoW%2Fb2YNEauDCaQ%3D
#include <iostream>
#include <cstdlib>
#include <mpi.h>
#include <string>
#include <math.h>
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


int
main (int argc, char* argv[])
{
  stopwatch_init ();
  struct stopwatch_t* timer;
  timer = stopwatch_create ();
  stopwatch_start (timer);

  //MPI Initialization
  int rank=0, np=0, namelen=0;
  char hostname[MPI_MAX_PROCESSOR_NAME+1];

  MPI_Init (&argc, &argv);	/* starts MPI */
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);	/* Get process id */
  MPI_Comm_size (MPI_COMM_WORLD, &np);	/* Get number of processes */
  MPI_Get_processor_name (hostname, &namelen); /* Get hostname of node */

  //get timer
  double start, end;

  if(rank == 0)
  {
    start = MPI_Wtime();
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

  gil::rgb8_image_t img(height, width);
  auto img_view = gil::view(img);

  //Creating a receiver buffer and receives buffer size
  float **final_image = new float *[height];
  float *recv_buffer = new float [width*height];
  for(int i = 0; i < height; i++)
  {
    final_image[i] = new float[width];
  }
  //Mandelbrot parallel code here
  float *local_mandelbrot_values = new float[(height*width)/np];
  y = minY + rank*(height/np)*it;
  for(int i = 0; i < height/np; ++i)
  {
    x = minX;
    for(int j = 0; j < width; ++j)
    {
      local_mandelbrot_values[i*width+j] = (mandelbrot(x,y)/512.0);
      x += jt;
    }
    y += it;
  }
 //Gathering
  MPI_Gather(local_mandelbrot_values, (height/np)*width, MPI_FLOAT, recv_buffer, (height/np)*width, MPI_FLOAT, 0, MPI_COMM_WORLD);

  if(rank == 0)
  {
    for (int i = 0; i < height; ++i)
    {
      for (int j = 0; j < width; ++j)
      {
        final_image[i][j] = recv_buffer[i*width+j];
        img_view(j, i) = render(final_image[i][j]);
      }
    }
    char *filename = new char[50];
    sprintf(filename, "mandelbrot_joe_%d_%dx%d.png", np, height, width);
    gil::png_write_view(filename, const_view(img));
  }

  //finalize
  MPI_Finalize();

  
  if(rank == 0)
  {
    stop = MPI_Wtime() - start;
    printf ("Time: %Lg seconds",stop);
    printf("Generating image of size %dx%d using %d processes\n", height, width, np);
    printf("Mandelbrot Image Generation using Joe Block's Logic finished!\n\n");
  }
  return 0;
}

  /* Lucky you, you get to write MPI code */


/* eof */
