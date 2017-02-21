#!/bin/bash
#$ -N Mandelbrot
#$ -q eecs117,pub8i
#$ -pe mpi 64
#$ -R y

# Grid Engine Notes:
# -----------------
# 1) Use "-R y" to request job reservation otherwise single 1-core jobs
#    may prevent this multicore MPI job from running.   This is called
#    job starvation.

# Module load boost
module load boost/1.57.0

# Module load OpenMPI
module load openmpi-1.8.3/gcc-4.9.2


echo "Script began:" `date`
echo "Node:" `hostname`
echo "Current directory: ${PWD}"

echo ""
echo "=== Starting mandelbrot_serial ==="
# run the serial program
echo ""
echo " Dimensions : 10000 x 10000 "
mpirun -np 1 ./mandelbrot_serial 10000 10000

echo ""
echo "=== Starting mandelbrot_joe ==="
echo ""

echo ""
echo " threads: 4 "
echo " Dimensions : 10000 x 10000 "
# Run the program 
mpirun -np 4  ./mandelbrot_joe 10000 10000

echo ""
echo " threads: 16 "
echo " Dimensions : 10000 x 10000 "
# Run the program 
mpirun -np 16  ./mandelbrot_joe 10000 10000

echo ""
echo " threads: 32 "
echo " Dimensions : 10000 x 10000 "
# Run the program 
mpirun -np 32  ./mandelbrot_joe 10000 10000

echo ""
echo " threads: 64 "
echo " Dimensions : 10000 x 10000 "
# Run the program 
mpirun -np 64  ./mandelbrot_joe 10000 10000

echo ""
echo "=== Starting mandelbrot_susie ==="
echo ""
echo " threads: 4 "
echo " Dimensions : 10000 x 10000 "
# Run the program 
mpirun -np 4  ./mandelbrot_susie 10000 10000

echo ""
echo " threads: 16 "
echo " Dimensions : 10000 x 10000 "
# Run the program 
mpirun -np 16  ./mandelbrot_susie 10000 10000

echo ""
echo " threads: 32 "
echo " Dimensions : 10000 x 10000 "
# Run the program 
mpirun -np 32  ./mandelbrot_susie 10000 10000

echo ""
echo " threads: 64 "
echo " Dimensions : 10000 x 10000 "
# Run the program 
mpirun -np 64  ./mandelbrot_susie 10000 10000

echo ""
echo "=== Starting mandelbrot_ms ==="
echo ""

echo ""
echo " threads: 4 "
echo " Dimensions : 10000 x 10000 "
# Run the program 
mpirun -np 4  ./mandelbrot_ms 10000 10000

echo ""
echo " threads: 16 "
echo " Dimensions : 10000 x 10000 "
# Run the program 
mpirun -np 16  ./mandelbrot_ms 10000 10000

echo ""
echo " threads: 32 "
echo " Dimensions : 10000 x 10000 "
# Run the program 
mpirun -np 32  ./mandelbrot_ms 10000 10000

echo ""
echo " threads: 64 "
echo " Dimensions : 10000 x 10000 "
# Run the program 
mpirun -np 64  ./mandelbrot_ms 10000 10000

echo ""
echo "=== Done! ==="
