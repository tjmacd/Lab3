#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#define BUFFER_LEN 256

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

void readInSudoku (struct sudoku *sudoku, char fileName[]) {
	FILE *file = fopen(fileName, "r"); // open puzzle.txt for reading
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
	} else {
		printf("Could not open file\n");
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

void validateSudoku (struct sudoku *sudoku) {
	pthread_t columns, rows, grids;

	// create threads and execute thread functions
	pthread_create(&columns, NULL, validateColumns, (void *) sudoku);
	pthread_create(&rows, NULL, validateRows, (void *) sudoku);
	pthread_create(&grids, NULL, validateGrids, (void *) sudoku);

	pthread_join(columns, NULL);
	pthread_join(rows, NULL);
	pthread_join(grids, NULL);
}

int isGridFull (char puzzle[][], int row, int col) {
	for (int row = 0; row < 9; row+=3) {
		for (int col = 0; col < 9; col+=3) {
			if (puzzle[row][col] == 0)
				return 0;
		}
	}
	return 1;
}

int isAvailable (char puzzle[][], int row, int col, int value) {
	for (int i = 0; i < 9; i++) {
		if (puzzle[row][i] == (value+'0'))
			return 0;
		else if (puzzle[i][col] == (value+'0'))
			return 0
		/*-----TODO: Check for all values in the grid-----*/
	}
	return 1;
}

void solveSudoku (struct sudoku *sudoku) {
	// create copy of the puzzle in sudoku
	char puzzle[9][9];
	for (int row = 0; row < 9; row++) {
		for (int col = 0; col < 9; col++) {
			puzzle[row][col] = sudoku->puzzle[row][col];
		}
	}

	int value = 1, row = 0, col = 0;
	/*-----TODO: Implement algorithm-----*/
	if (!isGridFull(puzzle, row, col)) {

	}
}

int main (void) {
	// variable declaration
	struct sudoku *sudoku;
	// intitialize sudoku
	sudoku = calloc (108, sizeof(char));

	readInSudoku(sudoku, "puzzle.txt"); // read in puzzle and store in sudoku
	solveSudoku(sudoku);
	validateSudoku(sudoku);
	printSudoku(sudoku);

	free(sudoku); // free memory
}
