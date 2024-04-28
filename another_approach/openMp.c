#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <stdbool.h>
#include <time.h> // for strand: random matrix generation

#define ROWS 1000
#define COLUMNS 1000

void generateMatrix(char a[ROWS][COLUMNS]) {
	for(int i = 0; i < ROWS; i++)
		for(int j = 0; j < COLUMNS; j++)
			a[i][j] = (rand() % 26) + 'A'; // Generating random capital letters
}

bool isPalindrome(char* str, int len)
{
	for (int i = 0; i < len / 2; i++)
		if (str[i] != str[len - i - 1])
			return false;
	return true;
}

int searchPalindromes(char a[ROWS][COLUMNS], int n, int num_threads)
{
	int count = 0;
	char buffer[n+1];

	#pragma omp parallel for schedule(dynamic) num_threads(num_threads) private(buffer) reduction(+:count)
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLUMNS - n + 1; j++)
		{
			// Horizontal (left-right)
			strncpy(buffer, &a[i][j], n);
			buffer[n] = '\0';
			if (isPalindrome(buffer, n)) count++;

			// vertical (top-down)
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

int main()
{
	char matrix[ROWS][COLUMNS];
	srand(time(NULL)); // ensures random nums generated during @ execution
	generateMatrix(matrix);

	int threads = 5;
	int size = 4;
	double start = omp_get_wtime();
	int count3 = searchPalindromes(matrix, size, threads);
	double end = omp_get_wtime();
	printf("%d palindromes of size %d found in %f s. using %d threads.\n", count3, size, end - start, threads);

	return 0;
}
