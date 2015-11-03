#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#define BUFFER_LEN 256
#define PUZZLETXT "validPuzzle.txt"

struct sudoku {
	char puzzle[9][9];
	char columnFlags[9];
	char rowFlags[9];
	char gridFlags[9];
};

void printSudoku (struct sudoku *sudoku) {
	printf("Puzzle:\n");
	for (int row = 0; row < 9; row++) {
		for (int col = 0; col < 9; col++) {
			printf("%c ", sudoku->puzzle[row][col]);
		}
		printf ("\n");
	}

	printf("\nColumn flags:\n");
	for (int col = 0; col < 9; col++) {
		printf("%d ", sudoku->columnFlags[col]);
	}

	printf("\nRow flags:\n");
	for (int row = 0; row < 9; row++) {
		printf("%d ", sudoku->rowFlags[row]);
	}

	printf("\nGrid flags:\n");
	for (int grid = 0; grid < 9; grid++) {
		printf("%d ", sudoku->gridFlags[grid]);
	}
	printf("\n");
}

void readInPuzzle (struct sudoku *sudoku) {
	FILE *file = fopen(PUZZLETXT, "r"); // open puzzle.txt for reading
	if (file != NULL) {
		char line[19]; // length 19 in order to hold 9 chars, 9 spaces, and 1 newline character
		int lineSize = (sizeof(line) / sizeof(line[0])); // get lineSize

		for (int row = 0; row < 9; row++) { // loop for each row of the puzzle
			if(fgets(line, lineSize, file) != NULL) {
				 // loop for every-other value in the line, starting from 0
				for (int col = 0; col < lineSize-2; col+=2) { // use lineSize-2 as end to eliminate newline char
					sudoku->puzzle[row][col/2] = line[col];
				}
			}
		}
	}
	fclose(file);
}

void* validateColumns (void* arg) {
	struct sudoku *sudoku = (struct sudoku*) arg;

	// for each column
	for (int col = 0; col < 9; col++) {
		int value = 1;
		// repeat search 9 times
		for (int j = 0; j < 9; j++) {
			// for each row in the column
			for (int row = 0; row < 9; row++) {
				if (sudoku->puzzle[row][col] == (value+'0')) {
					value++;
					break;
				}
			}
		}
		if (value == 10) { // if 1-9 was found in the column
			sudoku->columnFlags[col] = 1; // set flag to 1
		} else {
			sudoku->columnFlags[col] = 0; // else set flag to 0
		}
	}

	return NULL;
}

void* validateRows (void* arg) {
	struct sudoku *sudoku = (struct sudoku*) arg;

	// for each row
	for (int row = 0; row < 9; row++) {
		int value = 1;
		// repeat search 9 times
		for (int j = 0; j < 9; j++) {
			// for each column in the row
			for (int col = 0; col < 9; col++) {
				if (sudoku->puzzle[row][col] == (value+'0')) {
					value++;
					break;
				}
			}
		}
		if (value == 10) { // if 1-9 was found in the column
			sudoku->rowFlags[row] = 1; // set flag to 1
		} else {
			sudoku->rowFlags[row] = 0; // else set flag to 0
		}
	}

	return NULL;
}

int validateGrid (struct sudoku *sudoku, int rowNum, int column) {
	int value = 1;

	for (int k = 0; k < 9; k++) {
		for (int row = rowNum; row < rowNum+3; row++) {
			for (int col = column; col < column+3; col++) {
				if (sudoku->puzzle[row][col] == (value+'0')) {
					value++;
					break;
				}
			}
		}
	}
	if (value == 10) {
		return 1;
	} else {
		return 0;
	}
}

void* validateGrids (void* arg) {
	struct sudoku *sudoku = (struct sudoku*) arg;

	for (int row = 0; row < 9; row+=3) {
		for (int col = 0; col < 9; col+=3) {
			sudoku->gridFlags[row+col/3] = validateGrid(sudoku, row, col);
		}
	}

	return NULL;
}

int main (void) {
	// variable declaration
	struct sudoku *sudoku;
	pthread_t columns, rows, grids;

	// intitialize sudoku
	sudoku = calloc (108, sizeof(char));

	readInPuzzle(sudoku); // read in puzzle and store in sudoku

	// create threads and execute thread functions
	pthread_create(&columns, NULL, validateColumns, (void *) sudoku);
	pthread_create(&rows, NULL, validateRows, (void *) sudoku);
	pthread_create(&grids, NULL, validateGrids, (void *) sudoku);

	pthread_join(columns, NULL);
	pthread_join(rows, NULL);
	pthread_join(grids, NULL);

	//printSudoku(sudoku);

	free(sudoku); // free memory
}
