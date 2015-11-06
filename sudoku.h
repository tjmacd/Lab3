/*
 * Sudoku Solver for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, Muhammad Ahmad, Timothy MacDougall, Devin Westbye
 * All rights reserved.
 * 
 */
extern void printSudoku ();
extern void writeToFile (char fileName[]);
extern void readInSudoku (char fileName[]);
extern void* validateColumns ();
extern void* validateRows ();
extern int validateGrid (int rowNum, int column);
extern void* validateGrids ();
extern int validateSudoku ();
extern int check_row(int row, int num);
extern int check_col(int col, int num);
extern int check_grid(int row, int col, int num);
extern void navigate(int row, int col);
extern void solve_square(int row, int col);
