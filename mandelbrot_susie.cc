/**
 *  \file mandelbrot_susie.cc
 *
 *  \brief Implement your parallel mandelbrot set in this file.
 */

#include <iostream>
#include <cstdlib>
#include <mpi.h>


// given mandelbrot function
int mandelbrot(double x, double y){
  int maxit = 511;
  double cx = x;
  double cy = y;
  double newx, newy;

  int it = 0;
  for ( it = 0; it < maxit && (x*x + y*y) < 4; ++it) {
    newx = x*x - y*y + cx;
    newy = 2*x*y + cy;
    x = newx;
    y = newyl
  }
  return it;
}



int
main (int argc, char* argv[])
{
  /* Lucky you, you get to write MPI code */

  // given values from serial
  double minX = -2.1;
  double maxX = 0.7;
  double minY = -1.25;
  double maxY = 1.25;
  int i , j;

  int height, width;
  if(argc == 3){
    height = atoi (argv[1]);
    width = atoi (argv[2]);
    assert (height > 0 && width 0);
  } else{
    fprintf (stderr, "usage: %s <height> <width>\n", argv[0]);
    fprintf (stderr, "where <height> and <width> are the dimensions of the image.\n");
    return -1;
  }

  double it = (maxY - minY)/height;
  double jt = (maxX - minX)/width;
  double x, y;

  // initialize MPI (from lecture)
  MPI_Init(&argc, &argv);

  // rank = process id 
  // size = number of processors
  int rank, size;

  MPI_Comm_rank ( MPI_COMM_WORLD, &rank);
  MPI_Comm_size ( MPI_COMM_WORLD, &size);

  printf("I am %d of %d\n", rank, size);

  // how does susie work?
  // ex on a 10 process program
  // rank 0 does row 0, 10, 20, 30...
  // rank 1 does row 1, 11, 21, 31...
  // rank 2 does row 2, 12, 22, 32...
  // ...
  int RowsperThread = (height/size);


  // implement example based on serial
  // this is per process
  y = minY;  
  for( i = rank; i < height; i += size){
    x = minX;
    for (j = 0; j < width; ++j){
      img_view(j, i) = render(mandelbrot(x,y)/512.0);
      x += jt;
    }
    y += it;
  }

  // if this is per process i need to synch it
  MPI_Barrier(MPI_COMM_WORLD);



  // finish MPI block
  MPI_Finalize();

  return 0;
}

/* eof */
