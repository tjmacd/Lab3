/*
 * Sudoku Solver for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, Muhammad Ahmad, Timothy MacDougall, Devin Westbye
 * All rights reserved.
 * 
 */
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include "sudoku.h"
#define BUFFER_LEN 256

int puzzle[9][9];
int columnFlags[9];
int rowFlags[9];
int gridFlags[9];
bool is_filled = false;

// Prints the sudoku to the console
void printSudoku () {
	printf("Puzzle:\n");
	for (int row = 0; row < 9; row++) {
		for (int col = 0; col < 9; col++) {
			printf("%d ", puzzle[row][col]);
		}
		printf ("\n");
	}

	printf("\nColumn flags:\n");
	for (int col = 0; col < 9; col++) {
		printf("%d ", columnFlags[col]);
	}

	printf("\nRow flags:\n");
	for (int row = 0; row < 9; row++) {
		printf("%d ", rowFlags[row]);
	}

	printf("\nGrid flags:\n");
	for (int grid = 0; grid < 9; grid++) {
		printf("%d ", gridFlags[grid]);
	}
	printf("\n");
}

// Writes the sudoku to the given file
void writeToFile (char fileName[]) {
	FILE *file = fopen(fileName, "w");

	if (file != NULL) {
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				if (j != 8)
					// print int with space
					fprintf(file, "%d ", puzzle[i][j]); 
				else
				// print int without space for last value in line
					fprintf(file, "%d", puzzle[i][j]); 
			}
			// print newline in file after 9 values have been 
			// written on a line
			fprintf(file, "\n"); 
		}
	} else {
		printf("File: %s could not be opened for writing", fileName);
		return;
	}
}

// Reads sudoku from given file
void readInSudoku (char fileName[]) {
	// open puzzle.txt for reading
	FILE *file = fopen(fileName, "r"); 
	if (file != NULL) {
		for (int row = 0; row < 9; row++) { // loop for each row 
			for (int col = 0; col < 9; col++) {
				fscanf(file, "%d ", &puzzle[row][col]);
			}
		}
	} else {
		printf("Could not open file\n");
	}
	fclose(file);
}

// Check that columns fit the sudoku constraints
void* validateColumns () {
	// for each column
	for (int col = 0; col < 9; col++) {
		int value = 1;
		// repeat search 9 times
		for (int j = 0; j < 9; j++) {
			// for each row in the column
			for (int row = 0; row < 9; row++) {
				if (puzzle[row][col] == value) {
					value++;
					break;
				}
			}
		}
		if (value == 10) { // if 1-9 was found in the column
			columnFlags[col] = 1; // set flag to 1
		} else {
			columnFlags[col] = 0; // else set flag to 0
		}
	}
	return NULL;
}

// Check that rows obey the sudoku constraints
void* validateRows () {
	// for each row
	for (int row = 0; row < 9; row++) {
		int value = 1;
		// repeat search 9 times
		for (int j = 0; j < 9; j++) {
			// for each column in the row
			for (int col = 0; col < 9; col++) {
				if (puzzle[row][col] == value) {
					value++;
					break;
				}
			}
		}
		if (value == 10) { // if 1-9 was found in the column
			rowFlags[row] = 1; // set flag to 1
		} else {
			rowFlags[row] = 0; // else set flag to 0
		}
	}
	return NULL;
}

// Check that grid obeys sudoku constraints
int validateGrid (int rowNum, int column) {
	int value = 1;

	for (int k = 0; k < 9; k++) {
		for (int row = rowNum; row < rowNum+3; row++) {
			for (int col = column; col < column+3; col++) {
				if (puzzle[row][col] == value) {
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

// Check that all grids obey sudoku constraints
void* validateGrids () {
	for (int row = 0; row < 9; row+=3) {
		for (int col = 0; col < 9; col+=3) {
			gridFlags[row+col/3] = validateGrid(row, col);
		}
	}
	return NULL;
}

// Check that the sudoku is correct
int validateSudoku () {
	pthread_t columns, rows, grids;

	// create threads and execute thread functions
	pthread_create(&columns, NULL, validateColumns, NULL);
	pthread_create(&rows, NULL, validateRows, NULL);
	pthread_create(&grids, NULL, validateGrids, NULL);

	pthread_join(columns, NULL);
	pthread_join(rows, NULL);
	pthread_join(grids, NULL);

	for (int i = 0; i < 9; i++) {
		if (columnFlags[i] == 0)
			return 0;
		if (rowFlags[i] == 0)
			return 0;
		if (gridFlags[i] == 0)
			return 0;
	}
	return 1;
}

// Checks if num can be inserted in row
int check_row(int row, int num)
{
	for (int col=0; col < 9; col++){
		if(puzzle[row][col]==num){
			return 0; //number is already in row
		}
	}
	return 1; //Number is not found in row
}

// Checks if num can be inserted in col
int check_col(int col, int num){
	for(int row=0; row < 9; row++){
		if(puzzle[row][col]==num){
			return 0; //Number is already in column
		}
	}
	return 1; //Number is not in column
}

//Checks if we can put the number in the 3*3 grid
//Takes the starting row and col
int check_grid(int row, int col, int num)
{
	row=(row/3)*3;
	col=(col/3)*3;
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			if(puzzle[row+i][col+j]==num){
				return 0; //Number is already in grid
			}
		}
	}
	return 1; //Number is not yet in grid
}

//Move to the next cell if we have filled one cell
void navigate(int row, int col)
{
	if(col<8)
		solve_square(row, col+1);
	else
		solve_square(row+1, 0);
}

// Solve the sudoku using backtracking algorithm, start at 0, 0
void solve_square(int row, int col) {
	if(row>8){
		is_filled = true;
	}
	if(puzzle[row][col] != 0){
		navigate(row, col);
	} else {
		for(int i = 1; i <= 9; i++){
			if((check_row(row,i)==1) && (check_col(col,i)==1) &&
				check_grid(row, col, i)==1){
				puzzle[row][col] = i;
				navigate(row, col);
			}
		}
		if(!is_filled)
			//No valid number found, backtrack
			puzzle[row][col] = 0;
	}
}

int main (void) {
	readInSudoku("puzzle.txt"); // read in puzzle and store in sudoku
	solve_square(0,0);
	if (validateSudoku() == 1) {
		printSudoku();
		writeToFile("solution.txt");
	} else {
		printf("Puzzle invalid");
	}
}
