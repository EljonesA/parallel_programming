#include <stdio.h>
#include <string.h> // for malloc
#include <stdlib.h>
#include <time.h> // for strand
#include <omp.h>

#define ROWS 10
#define COLS 10

/* >>>>>>>>> 2D MATRIXES <<<<<<<<<< */
// this function generates random chars btwn a & z
char random_char()
{
	return 'a' + (rand() % 26);
}

void random_matrix(char matrix[ROWS][COLS])
{
	int i, j;

	// populate matrix with random chars
	for (i = 0; i < ROWS; i++)
	{
		for (j = 0; j < COLS; j++)
			matrix[i][j] = random_char();
	}
}

void print_matrix(char matrix[ROWS][COLS])
{
	int i, j;

	for (i = 0; i < ROWS; i++)
	{
		printf("["); // ROW open brackets
		for (j = 0; j < COLS; j++)
		{
			printf("%c", matrix[i][j]);
			if ( j != COLS - 1)
				printf(", "); // comma btwn elements
		}
		printf("]\n"); // ROW close brackets
	}
}

int is_palindrome(char *str, int length)
{
	int i;

	for (i = 0; i < length / 2; i++)
	{
		if (str[i] != str[length - i  -1])
			return 0;
	}
	return 1;
}

void search_palindrome(char matrix[ROWS][COLS], int n)
{
	char substring[n + 1];
	int i, j, k;
	int count = 0;
	double start_time = omp_get_wtime();

	// horizontal (right to left) search
	#pragma omp parallel for private(i, j, substring) reduction(+:count)
	for (i = 0; i < ROWS; i++)
	{
		for (j = 0; j < COLS - n; j++)
		{
			strncpy(substring, &matrix[i][j], n);
			substring[n] = '\0';
			if (is_palindrome(substring, n))
				printf("Hoizontal: Palindrome found at position (%d, %d): %s\n", i, j, substring);
				count++; // incrment count when palindrome is found
		}
	}

	// vertical (up to down) search
    	#pragma omp parallel for private(i, j, substring) reduction(+:count)
	for (j = 0; j < COLS; j++)
	{
		for (i = 0; i < ROWS - n + 1; i++)
		{
			for (k = 0; k < n; k++)
				substring[k] = matrix[i + k][j];
		     	substring[n] = '\0';
			if (is_palindrome(substring, n))
			printf("Vertical: Palindrome found at position (%d, %d): %s\n", i, j, substring);
			count++; //increment count when palindrome is found
		}
	}

	// diagonal (top-left to bottom-right) search
	#pragma omp parallel for private(i, j, substring) reduction(+:count)
	for (i = 0; i <= ROWS - n; i++)
	{
		for (j = 0; j <= COLS - n; j++)
		{
			for (k = 0; k < n; k++)
				substring[k] = matrix[i + k][j + k];
			substring[n] = '\0';
			if (is_palindrome(substring, n))
			printf("Diagonal: Palindrome found at position (%d, %d): %s\n", i, j, substring);
			count++; // increment count when palindrome is found
		}
	}

	double end_time = omp_get_wtime();
	double execution_time = end_time - start_time;

	printf("%d palindromes of size %d found in %f s. using %d threads.\n", count, n, execution_time, omp_get_max_threads());
}

int main()
{
	// matrix
	char matrix[ROWS][COLS];
	int palindrome_length = 6;

	srand(time(NULL)); // ensures random nums generated during @ execution
	random_matrix(matrix);

	// print the random matrix
	printf("Random Matrix:\n");
	print_matrix(matrix);
	printf("\n");

	printf("Searching for palindromes of length %d:\n", palindrome_length);
	search_palindrome(matrix, palindrome_length);

	return 0;
}
