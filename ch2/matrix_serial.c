#include <stdio.h>
#define ARRAY_SIZE 10

typedef int matrix_t[ARRAY_SIZE][ARRAY_SIZE];
matrix_t MA, MB, MC;

void mult(int size,		/* size of the matrix */
		  int row,		/* row of result to compute */
		  int column,	/* column of result to compute */
		  matrix_t MA,  /* input matrix */
		  matrix_t MB,  /* input matrix */
		  matrix_t MC) { /* result matrix */
	
	int position;

	MC[row][column] = 0;
	for(position = 0; position < size; position++) {
		MC[row][column] = MC[row][column] + (MA[row][position]*MB[position][column]);
	
	}
}

main() {
	int size = ARRAY_SIZE, row, column;
	/* Fill in matrix values */

	/* process matrix by row and column */
	for(row = 0; row < size; row++) {
		for(column = 0; column < size; column++) {
			mult(size, row, column, MA, MB, MC);
		}
	}
	/* print */
}
