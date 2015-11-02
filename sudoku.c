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
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			printf("%c ", sudoku->puzzle[i][j]);
		}
		printf ("\n");
	}

	printf("\nColumn flags:\n");
	for (int i = 0; i < 9; i++) {
		printf("%d ", sudoku->columnFlags[i]);
	}

	printf("\nRow flags:\n");
	for (int i = 0; i < 9; i++) {
		printf("%d ", sudoku->rowFlags[i]);
	}

	printf("\nGrid flags:\n");
	for (int i = 0; i < 9; i++) {
		printf("%d ", sudoku->gridFlags[i]);
	}
	printf("\n");
}

void readInPuzzle (struct sudoku *sudoku) {
	FILE *file = fopen(PUZZLETXT, "r"); // open puzzle.txt for reading
	if (file != NULL) {
		char line[19]; // length 19 in order to hold 9 chars, 9 spaces, and 1 newline character
		int lineSize = (sizeof(line) / sizeof(line[0])); // get lineSize

		for (int i = 0; i < 9; i++) { // loop for each row of the puzzle
			if(fgets(line, lineSize, file) != NULL) {
				 // loop for every-other value in the line, starting from 0
				for (int j = 0; j < lineSize-2; j+=2) { // use lineSize-2 as end to eliminate newline char
					sudoku->puzzle[i][j/2] = line[j];
				}
			}
		}
	}
	fclose(file);
}

void* validateColumns (void* arg) {
	struct sudoku *sudoku = (struct sudoku*) arg;

	// for each column
	for (int i = 0; i < 9; i++) {
		int index = 1;
		// repeat search 9 times
		for (int j = 0; j < 9; j++) {
			// for each row in the column
			for (int k = 0; k < 9; k++) {
				if (sudoku->puzzle[k][i] == (index+'0')) {
					index++;
					break;
				}
			}
		}
		if (index == 10) { // if 1-9 was found in the column
			sudoku->columnFlags[i] = 1; // set flag to 1
		} else {
			sudoku->columnFlags[i] = 0; // else set flag to 0
		}
	}

	return NULL;
}

void* validateRows (void* arg) {
	struct sudoku *sudoku = (struct sudoku*) arg;

	// for each column
	for (int i = 0; i < 9; i++) {
		int index = 1;
		// repeat search 9 times
		for (int j = 0; j < 9; j++) {
			// for each row in the column
			for (int k = 0; k < 9; k++) {
				if (sudoku->puzzle[i][k] == (index+'0')) {
					index++;
					break;
				}
			}
		}
		if (index == 10) { // if 1-9 was found in the column
			sudoku->rowFlags[i] = 1; // set flag to 1
		} else {
			sudoku->rowFlags[i] = 0; // else set flag to 0
		}
	}

	return NULL;
}

void* validateGrids (void* arg) {
	struct sudoku *sudoku = (struct sudoku*) arg;

	

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

	printSudoku(sudoku);

	free(sudoku); // free memory
}
