#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#define BUFFER_LEN 256
#define PUZZLETXT "puzzle.txt"

void readInPuzzle (char puzzle[9][9]) {
	FILE *file = fopen(PUZZLETXT, "r"); // open puzzle.txt for reading
	if (file != NULL) {
		char line[19]; // length 19 in order to hold 9 chars, 9 spaces, and 1 newline character

		for (int i = 0; i < 9; i++) { // loop for each row of the puzzle
			int lineSize = (sizeof(line) / sizeof(line[0])); // get lineSize

			if(fgets(line, lineSize, file) != NULL) {
				 // loop for every-other value in the line, starting from 0
				for (int j = 0; j < lineSize-2; j+=2) { // use lineSize-2 to eliminate newline char
					puzzle[i][j/2] = line[j];
				}
			}
		}
	}
	fclose(file);
}

int main (void) {
	// variable declaration
	char puzzle[9][9];

	readInPuzzle(puzzle);
}
