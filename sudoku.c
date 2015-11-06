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
	for (int row = 0; row < 9; row++) {
		for (int col = 0; col < 9; col++) {
			printf("%d ", puzzle[row][col]);
		}
		printf ("\n");
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
	for(int col = 0; col < 9; col++){
		int counts[9] = {0};
		for(int row = 0; row < 9; row++){
			if(puzzle[row][col] != 0){
				counts[puzzle[row][col]-1] += 1;
			}
		}
		columnFlags[col] = 1;
		for(int i = 0; i < 9; i++){
			if(counts[i] > 1){
				columnFlags[col] = 0;
				break;
			}
		}
	}
	return NULL;
}

// Check that rows obey the sudoku constraints
void* validateRows () {
	for(int row = 0; row < 9; row++){
		int counts[9] = {0};
		for(int col = 0; col < 9; col++){
			if(puzzle[row][col] != 0){
				counts[puzzle[row][col]-1] += 1;
			}
		}
		rowFlags[row] = 1;
		for(int i = 0; i < 9; i++){
			if(counts[i] > 1){
				rowFlags[row] = 0;
				break;
			}
		}
	}
	return NULL;
}

// Check that grid obeys sudoku constraints
int validateGrid (int rowNum, int column) {
	int counts[9] = {0};
	for(int row = rowNum; row < rowNum+3; row++){
		for(int col = column; col < column+3; col++){
			if(puzzle[row][col] != 0){
				counts[puzzle[row][col]-1] += 1;
			}
		}
	}
	for(int i = 0; i < 9; i++){
		if(counts[i] > 1){
			return 0;
		}
	}
	return 1;
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
		return;
	}
	if(puzzle[row][col] != 0){
		navigate(row, col);
	} else {
		for(int i = 1; i <= 9; i++){
			puzzle[row][col] = i;
			validateSudoku();
			if(columnFlags[col] != 1 || rowFlags[row] != 1 
				|| gridFlags[(row/3)*3+col/3] != 1){
				puzzle[row][col] = 0;
			} else {
				navigate(row, col);
			}
			if(is_filled)
				return;
		}
		//No valid number found, backtrack
		puzzle[row][col] = 0;
	}
}

int main (void) {
	readInSudoku("puzzle.txt"); // read in puzzle and store in sudoku
	//Check that puzzle is valid
	validateSudoku();
	bool valid = true;
	for(int i = 0; i < 9; i++){
		if(columnFlags[i] == 0 || rowFlags[i] == 0 || gridFlags[i] == 0){
			valid = false;
			break;
		}
	}
	if(valid){
		puts("Unsolved Puzzle:");
		printSudoku();
		solve_square(0,0);
		if(is_filled){
			puts("Solved Puzzle:");
			printSudoku();
			writeToFile("solution.txt");
		} else {
			puts("Sudoku puzzle is unsolvable.");
		}
	} else {
		puts("Puzzle is not a valid sudoku.");
	}
}
