#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h> // for strand: random matrix gneration

#define ROWS 1000
#define COLUMNS 1000

void generateMatrix(char a[ROWS][COLUMNS]) {
	for(int i = 0; i < ROWS; i++)
		for(int j = 0; j < COLUMNS; j++)
			a[i][j] = (rand() % 26) + 'A';
}

bool isPalindrome(char* str, int len)
{
	for (int i = 0; i < len / 2; i++)
		if (str[i] != str[len - i - 1])
			return false;
	return true;
}

int searchPalindromes(char a[ROWS][COLUMNS], int n, int start_row, int end_row)
{
	int count = 0;
	char buffer[n+1];

	for (int i = start_row; i < end_row; i++)
	{
		for (int j = 0; j < COLUMNS - n + 1; j++)
		{
			// Horizontal (left-right)
			strncpy(buffer, &a[i][j], n);
			buffer[n] = '\0';
			if (isPalindrome(buffer, n)) count++;

			// Vertical (top-down)
			for (int k = 0; k < n; k++) buffer[k] = a[j+k][i];
			if (isPalindrome(buffer, n)) count++;

			// diagonal (top-left to bottom-right)
			if (i <= ROWS - n && j <= COLUMNS - n)
			{
				for (int k = 0; k < n; k++) buffer[k] = a[i+k][j+k];
				if (isPalindrome(buffer, n)) count++;
			}
		}
	}
	return count;
}

int main(int argc, char** argv)
{
	MPI_Init(&argc, &argv);

	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	char matrix[ROWS][COLUMNS];
	srand(time(NULL)); // ensures random nums generated during @ execution
	if (rank == 0)
	{
	       generateMatrix(matrix);
	}

	// Broadcast the matrix to all processes
	MPI_Bcast(matrix, ROWS*COLUMNS, MPI_CHAR, 0, MPI_COMM_WORLD);
	
	int rows_per_process = ROWS / size;
	int start_row = rank * rows_per_process;
	int end_row = (rank + 1) * rows_per_process;
	
	for (int palindrome_size = 3; palindrome_size <= 6; palindrome_size++)
	{
		double start = MPI_Wtime();
		int local_count = searchPalindromes(matrix, palindrome_size, start_row, end_row);
		int total_count;
		MPI_Reduce(&local_count, &total_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
		double end = MPI_Wtime();
	
		if (rank == 0) {
			printf("%d palindromes of size %d found in %f s. using %d threads.\n", total_count, palindrome_size, end - start, size);
		}
	}

	MPI_Finalize();
	return 0;
}
