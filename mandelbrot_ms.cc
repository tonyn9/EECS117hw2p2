/**
 *  \file mandelbrot_ms.cc
 *
 *  \brief Implement your parallel mandelbrot set in this file.
 */

#include <iostream>
#include <cstdlib>
#include <mpi.h>
#include <math.h>
#include "render.hh"

// rank = process id 
// size = number of processors
int rank, size;

// communication tags
// these are mapped out to integers
enum {
  finish_tag, // no more work for slaves
  data_tag // more work for slaves
};


int mpiInit(int argc, char**argv){
  if (MPI_Init(&argc, &argv) != MPI_SUCCESS){
    fprintf(stderr, "Failure in initialization\n");
    return 100;
  }

  if (MPI_Comm_size(MPI_COMM_WORLD, &size) != MPI_SUCCESS){
    fprintf(stderr, "Could not get process count\n");
    MPI_Finalize();
    return 101;
  }else if (size < 2){
    fprintf(stderr, "Must have at least 2 process\n");
    MPI_Finalize();
    return 102;
  }

  if (MPI_Comm_rank(MPI_COMM_WORLD, &rank) != MPI_SUCCESS){
    fprintf(stderr, "Could not get rank of process\n");
    return 103;
  }

  printf("Number of processes: %d\n", size);

  return 0;

}


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

// waits until told to work by root process
// when finished, return the data
int slave(double minX, double maxX, double minY, double maxY, double it, double jt, int height, int width){

  // process can finish before other process
  // need to communicate which row it finished
  // without needing to use another send and recieve

  // create buffer + row#
  int SendBuffer[width + 1];


  double x,y;

  // get row to work on
  // work on row
  // send row to root process

  while(true){
    
  }



  return 0;
}

// gives work out to other process and collects data at the same time
int master(double minX, double maxX, double minY, double maxY, double it, double jt, int height, int width){

  double start = MPI_Wtime();


  // create image buffer
  int ImageBuffer [height * width];

  // create recieve buffer + row#
  int RecieveBuffer [width + 1];


  // start giving processes work
  // height may be less than # of process
  while(){

  }

  // if there are more rows than processes
  // recieve the process and give more work
  while(){

  }

  // gather remaining processes 
  // stop giving work to processes
  while(){

  }

  // render the image



  printf("Finished in about %f seconds. \n", MPI_Wtime()-start);


  return 0;
}


int
main (int argc, char* argv[])
{
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

  // initialize MPI
  if (mpi_Init(argc, argv)){
    fprintf(stderr, "Could not init MPI\n");
    return 10;
  }

  // split process between one process (master)
  // and many other processes (slaves)

  // master goes here
  if (rank == 0){
      master(minX, maxX, minY, maxY, it, jt, height, width);
  }else{ // slave goes here
      slave(minX, maxX, minY, maxY, it, jt, height, width);
  }

  // finish MPI block
  MPI_Finalize();

  return 0;
}

/* eof */
