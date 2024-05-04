# OpenMp
- To set __palindrome size__ to search for and __threads__ to use,
- in main function (int main()) locate and edit **threads** & **size** variables

# Executing the file
- OpenMP: gcc -fopenmp -o omp_palindrome openMp.c
- Run OpenMP: ./omp_palindrome

# MPI
- To set __palindrome size__ to search for, in main function, locate and set the variable __palindrome_size
- The number of processes is set in command line, check **Run OpenMP** command.

# Executing the file
- MPI: mpicc -o mpi_palindrome mpi.c
- Run MPI: mpirun -np 2 ./mpi_palindrome       -> The 2 is the number of processes here
