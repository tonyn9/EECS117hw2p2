/**
 *  \file mandelbrot_susie.cc
 *
 *  \brief Implement your parallel mandelbrot set in this file.
 */

#include <iostream>
#include <cstdlib>
#include <mpi.h>
#include "render.hh"


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
    y = newy;
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
    assert (height > 0 && width > 0);
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

  //get timer
  double start, end;

  if(rank == 0){
    start = MPI_Wtime();
  }

  // rank = process id 
  // size = number of processors
  int rank, size;

  MPI_Comm_rank ( MPI_COMM_WORLD, &rank);
  MPI_Comm_size ( MPI_COMM_WORLD, &size);

  //printf("I am %d of %d\n", rank, size);

  // how does susie work?
  // ex on a 10 process program
  // rank 0 does row 0, 10, 20, 30...
  // rank 1 does row 1, 11, 21, 31...
  // rank 2 does row 2, 12, 22, 32...
  // ...

  // need to get the ceiling of a height/size
  int RowsperThread = height/size + 1;

  // create a send buffer
  int ProcLength = RowsperThread*width;
  int SendBuffer[RowsperThread*width];

  // implement example based on serial
  // this is per process
  y = minY + (rank * it);  
  int row = 0;
  for( i = rank; i < height; i += size){
    x = minX;
    for (j = 0; j < width; ++j){
      SendBuffer [(row * width) + j] = mandelbrot(x,y);
      x += jt;
    }
    y += (it*size);
    row +=1;
  }

  // if this is per process i need to synch it
  MPI_Barrier(MPI_COMM_WORLD);

  // need to gather data from the processes to root process

  // make a buffer for root process
  // needs a reference for all it seems
  int* ReceiveBuffer = NULL;
  if (rank == 0){
    ReceiveBuffer = new int [ProcLength * size];
  }
  
  MPI_Gather(SendBuffer,  ProcLength, MPI_INT, ReceiveBuffer,  ProcLength, MPI_INT, 0, MPI_COMM_WORLD);


  // root process makes image
  if (rank == 0){

    gil::rgb8_image_t img(height,width);
    auto img_view = gil::view(img);

    // need to get index and row from a 1d array
    int ProcessorIndex = 0;
    int RowIndex = 0;
    for ( i = 0; i < height; ++i){
      for (j = 0; j < width; ++j){
        ProcessorIndex = (i % size) * ProcLength; 
        img_view(j, i) = render(ReceiveBuffer[ProcessorIndex + (RowIndex*width) + j]/512.0);
      }
      RowIndex = i / size;
    }

    printf("Finished in about %f seconds. \n", MPI_Wtime()-start);

    gil::png_write_view("mandelbrot-susie.png", const_view(img));

  }

  // finish MPI block
  MPI_Finalize();

  return 0;
}

/* eof */
